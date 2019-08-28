/*
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
The following document shows functions and methods used to
manipulate and process Chapel bytes objects. The :record:`bytes` object is
similar to astringbut allows arbitrary data to be stored in it. Methods onbytes`
that interpret the data as characters assume that the bytes are ASCII characters

Casts from bytes to a Numeric Type
-----------------------------------

This module supports casts from :record:`bytes` to numeric types. Such casts
will interpret the bytes as ASCII characters and convert it to the numeric type
and throw an error if the bytes is invalid. For example:

.. code-block:: chapel

  var b = bytes("a");
  var number = b:int;

throws an error when it is executed, but

.. code-block:: chapel

  var b = bytes("1");
  var number = b:int;

stores the value ``1`` in ``number``.

To learn more about handling these errors, see the
:ref:`Error Handling technical note <readme-errorHandling>`.
 */
module Bytes {
  use ChapelStandard;
  use BytesCasts;
  private use ByteBufferHelpers;
  private use BytesStringCommon;

  /*
     ``DecodePolicy`` specifies what happens when there is malformed characters
     when decoding the :record:`bytes` object into a UTF-8 `string`.
       
       - **Strict**: default policy; raise error
       - **Replace**: replace with UTF-8 replacement character
       - **Ignore**: silently drop data
  */
  enum DecodePolicy { Strict, Replace, Ignore }

  /*
   A `DecodeError` is thrown if the `decode` method is called on a non-UTF-8
   string.
   */
  class DecodeError: Error {
    override proc message() {
      return "Invalid UTF-8 character encountered.";
    }
  }

  type idxType = int; 

  record _bytes {
    pragma "no doc"
    var len: int = 0; // length of string in bytes
    pragma "no doc"
    var _size: int = 0; // size of the buffer we own
    pragma "no doc"
    var buff: bufferType = nil;
    pragma "no doc"
    var isowned: bool = true;
    pragma "no doc"
    // We use chpl_nodeID as a shortcut to get at here.id without actually constructing
    // a locale object. Used when determining if we should make a remote transfer.
    var locale_id = chpl_nodeID; // : chpl_nodeID_t

    pragma "no doc"
    proc init() {

    }

    /*
      Initialize a new :record:`bytes` from ``s``. If ``isowned`` is set to
      ``true`` then ``b`` will be fully copied into the new instance. If it is
      ``false`` a shallow copy will be made such that any in-place modifications
      to the new bytes may appear in ``b``. It is the responsibility of the user
      to ensure that the underlying buffer is not freed while being used as part
      of a shallow copy.
     */
    proc init(s, isowned: bool = true) where s.type == string || s.type == bytes {
      const sRemote = _local == false && s.locale_id != chpl_nodeID;
      const sLen = s.len;
      this.isowned = isowned;
      this.complete();
      // Don't need to do anything if s is an empty string
      if sLen != 0 {
        this.len = sLen;
        if !_local && sRemote {
          // ignore supplied value of isowned for remote strings so we don't leak
          this.isowned = true;
          this.buff = bufferCopyRemote(s.locale_id, s.buff, sLen);
          this._size = sLen+1;
        } else {
          if this.isowned {
            const (buf, allocSize) = bufferCopyLocal(s.buff, sLen);
            this.buff = buf;
            this._size = allocSize;
          } else {
            this.buff = s.buff;
            this._size = s._size;
          }
        }
      }
    }

    pragma "no doc"
    proc ref deinit() {
      if isowned && this.buff != nil {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
          chpl_here_free(this.buff);
        }
      }
    }

    pragma "no doc"
    proc writeThis(f) {
      compilerError("not implemented: writeThis");
    }
    pragma "no doc"
    proc readThis(f) {
      compilerError("not implemented: readThis");
    }

    pragma "no doc"
    proc init=(b: bytes) {
      this.init(b);
    }

    pragma "no doc"
    proc init=(b: string) {
      this.init(b);
    }

    /*
      Initialize a new :record:`bytes` from the `c_string` `cs`. If `isowned` is
      set to true, the backing buffer will be freed when the new record is
      destroyed.  If `needToCopy` is set to true, the `c_string` will be copied
      into the record, otherwise it will be used directly. It is the
      responsibility of the user to ensure that the underlying buffer is not
      freed if the `c_string` is not copied in.
     */

    proc init(cs: c_string, length: int = cs.length,
                isowned: bool = true, needToCopy:  bool = true) {
      this.isowned = isowned;
      this.complete();
      const cs_len = length;
      this.reinitString(cs:bufferType, cs_len, cs_len+1, needToCopy);
    }

    /*
      Initialize a new :record:`bytes` from `buff` ( `c_ptr` ). `size` indicates
      the total size of the buffer available, while `len` indicates the current
      length of the string in the buffer (the common case would be `size-1` for
      a C-style string). If `isowned` is set to true, the backing buffer will be
      freed when the new record is destroyed. If `needToCopy` is set to true,
      the `c_string` will be copied into the record, otherwise it will be used
      directly. It is the responsibility of the user to ensure that the
      underlying buffer is not freed if the `c_string` is not copied in.
     */
    // This initializer can cause a leak if isowned = false and needToCopy = true
    proc init(buff: c_ptr, length: int, size: int,
                isowned: bool = true, needToCopy: bool = true) {
      //different than string's similar constructor. Here we are not limited to
      //any type
      this.isowned = isowned;
      this.complete();
      this.reinitString(buff, length, size, needToCopy);
    }

    // This is assumed to be called from this.locale
    pragma "no doc"
    proc ref reinitString(buf: bufferType, s_len: int, size: int,
                          needToCopy:bool = true) {
      if this.isEmpty() && buf == nil then return;

      /*const buf = _buf:bufferType; // this is different than string*/

      // If the this.buff is longer than buf, then reuse the buffer if we are
      // allowed to (this.isowned == true)
      if s_len != 0 {
        if needToCopy {
          if !this.isowned || s_len+1 > this._size {
            // If the new string is too big for our current buffer or we dont
            // own our current buffer then we need a new one.
            if this.isowned && !this.isEmpty() then
              bufferFree(this.buff);
            // TODO: should I just allocate 'size' bytes?
            const (buf, allocSize) = bufferAlloc(s_len+1);
            this.buff = buf;
            this._size = allocSize;
            // We just allocated a buffer, make sure to free it later
            this.isowned = true;
          }
          bufferMemmoveLocal(this.buff, buf, s_len);
          this.buff[s_len] = 0;
        } else {
          if this.isowned && !this.isEmpty() then
            bufferFree(this.buff);
          this.buff = buf;
          this._size = size;
        }
      } else {
        // If s_len is 0, 'buf' may still have been allocated. Regardless, we
        // need to free the old buffer if 'this' is isowned.
        if this.isowned && !this.isEmpty() then bufferFree(this.buff);
        this._size = 0;

        // If we need to copy, we can just set 'buff' to nil. Otherwise the
        // implication is that the string takes ownership of the given buffer,
        // so we need to store it and free it later.
        if needToCopy {
          this.buff = nil;
        } else {
          this.buff = buf;
        }
      }

      this.len = s_len;
    }

    /*
      :returns: The number of bytes in the object.
    */
    inline proc length return len;

    /*
      :returns: The number of bytes in the object.
      */
    inline proc size return len;

    /*
      :returns: The number of bytes in the bytes.
      */
    inline proc numBytes return len;

    /*
       Gets a version of the :record:`bytes` that is on the currently
       executing locale.

       :returns: A shallow copy if the :record:`bytes` is already on the
                 current locale, otherwise a deep copy is performed.
    */
    inline proc localize() : bytes {
      if _local || this.locale_id == chpl_nodeID {
        return createBytesWithBorrowedBuffer(this);
      } else {
        const x:bytes = this; // assignment makes it local
        return x;
      }
    }


    /*
      Get a `c_string` from a :record:`bytes`.
     */
    inline proc c_str(): c_string {
      return getCStr(this);
    }

    /*
      Gets a byte from the object

      :arg i: The index

      :returns: 1-length :record:`bytes` object
     */
    proc this(i: int): bytes {
      if boundsChecking && (i <= 0 || i > this.len)
        then halt("index out of bounds of bytes: ", i);
      var (buf, size) = bufferCopy(buf=this.buff, off=i-1, len=1,
                                   loc=this.locale_id);
      return createBytesWithOwnedBuffer(buf, length=1, size=size);
    }

    /*
      :returns: The value of the `i` th byte as an integer.
    */
    proc byte(i: int): byteType {
      if boundsChecking && (i <= 0 || i > this.len)
        then halt("index out of bounds of bytes: ", i);
      return bufferGetByte(buf=this.buff, off=i-1, loc=this.locale_id);
    }

    /*
      Iterates over the bytes

      :yields: 1-length :record:`bytes` objects
     */
    iter these(): bytes {
      if this.isEmpty() then return;
      for i in 1..this.len do
        yield this[i];
    }

    /*
      Iterates over the bytes byte by byte.
    */
    iter chpl_bytes(): byteType {
      for i in 1..this.len do
        yield this.byte(i);
    }

    /*
      Slices the object. Halts if r is not completely inside the range
      ``1..bytes.length`` when compiled with `--checks`. `--fast` disables this
      check.

      :arg r: range of the indices the new bytes should be made from

      :returns: a new bytes that is a slice within ``1..bytes.length``. If
                the length of `r` is zero, an empty bytes is returned.
     */
    proc this(r: range(?)) : bytes {
      return getSlice(this, r);
    }

    // Checks to see if r is inside the bounds of this and returns a finite
    // range that can be used to iterate over a section of the string
    // TODO: move into the public interface in some form? better name if so?
    pragma "no doc"
    proc _getView(r:range(?)) where r.idxType == int {
      if boundsChecking {
        if r.hasLowBound() && (!r.hasHighBound() || r.size > 0) {
          if r.low:int <= 0 then
            halt("range out of bounds of bytes");
        }
        if r.hasHighBound() && (!r.hasLowBound() || r.size > 0) {
          if (r.high:int < 0) || (r.high:int > this.len) then
            halt("range out of bounds of bytes");
        }
      }
      const r1 = r[1:r.idxType..this.len:r.idxType];
      if r1.stridable {
        const ret = r1.low:int..r1.high:int by r1.stride;
        return ret;
      } else {
        const ret = r1.low:int..r1.high:int;
        return ret;
      }
    }

    /*
      :returns: * `true`  -- when the object is empty
                * `false` -- otherwise
     */
    inline proc isEmpty() : bool {
      return this.len == 0;
    }

    /*
      :arg needles: A varargs list of :record:`bytes` objects to match against.

      :returns: * `true`  -- when the object begins with one or more of the `needles`
                * `false` -- otherwise
     */
    proc startsWith(needles: bytes ...) : bool {
      return startsEndsWith(this, needles, fromLeft=true);
    }

    /*
      :arg needles: A varargs list of :record:`bytes` objects to match against.

      :returns: * `true`  -- when the object ends with one or more of the `needles`
                * `false` -- otherwise
     */
    proc endsWith(needles: bytes ...) : bool {
      return startsEndsWith(this, needles, fromLeft=false);
    }

    /*
      :arg needle: the :record:`bytes` object to search for
      :arg region: an optional range defining the indices to search within,
                   default is the whole object. Halts if the range is not
                   within ``1..bytes.length``

      :returns: the index of the first occurrence of `needle` within a
                the object, or 0 if the `needle` is not in the object.
     */
    proc find(needle: bytes, region: range(?) = 1:idxType..) : idxType {
      return _search_helper(needle, region, count=false): idxType;
    }

    /*
      :arg needle: the :record:`bytes` to search for
      :arg region: an optional range defining the indices to search within,
                   default is the whole object. Halts if the range is not
                   within ``1..bytes.length``

      :returns: the index of the first occurrence from the right of `needle`
                within the object, or 0 if the `needle` is not in the object.
     */
    proc rfind(needle: bytes, region: range(?) = 1:idxType..) : idxType {
      return _search_helper(needle, region, count=false,
                            fromLeft=false): idxType;
    }

    /*
      :arg needle: the :record:`bytes` to search for
      :arg region: an optional range defining the substring to search within,
                   default is the whole object. Halts if the range is not
                   within ``1..bytes.length``

      :returns: the number of times `needle` occurs in the object
     */
    proc count(needle: bytes, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=true);
    }

    // Helper function that uses a param bool to toggle between count and find
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    //
    pragma "no doc"
    inline proc _search_helper(needle: bytes, region: range(?),
                               param count: bool, param fromLeft: bool = true) {
      // needle.len is <= than this.len, so go to the home locale
      var ret: int = 0;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        // any value > 0 means we have a solution
        // used because we cant break out of an on-clause early
        var localRet: int = -1;
        const nLen = needle.len;
        const view = this._getView(region);
        const thisLen = view.size;

        // Edge cases
        if count {
          if nLen == 0 { // Empty needle
            localRet = view.size+1;
          }
        } else { // find
          if nLen == 0 { // Empty needle
            if fromLeft {
              localRet = 0;
            } else {
              localRet = if thisLen == 0
                then 0
                else thisLen+1;
            }
          }
        }

        if nLen > thisLen {
          localRet = 0;
        }

        if localRet == -1 {
          localRet = 0;
          const localNeedle = needle.localize();
          const needleLen = localNeedle.len;

          // i *is not* an index into anything, it is the order of the element
          // of view we are searching from.
          const numPossible = thisLen - nLen + 1;
          const searchSpace = if fromLeft
              then 0..#(numPossible)
              else 0..#(numPossible) by -1;
          for i in searchSpace {
            const bufIdx = view.orderToIndex(i);
            const found = bufferEqualsLocal(buf1=this.buff, off1=bufIdx-1,
                                            buf2=localNeedle.buff, off2=0,
                                            len=needleLen);
            if found {
              if count {
                localRet += 1;
              } else { // find
                localRet = view.orderToIndex(i);
              }
            }
            if !count && localRet != 0 then break;
          }
        }
        ret = localRet;
      }
      return ret;
    }

    /*
      :arg needle: the :record:`bytes` to search for
      :arg replacement: the :record:`bytes` to replace `needle` with
      :arg count: an optional integer specifying the number of replacements to
                  make, values less than zero will replace all occurrences

      :returns: a copy of the object where `replacement` replaces `needle` up
                to `count` times
     */
    // TODO: not ideal - count and single allocation probably faster
    //                 - can special case on replacement|needle.length (0, 1)
    proc replace(needle: bytes, replacement: bytes, count: int = -1) : bytes {
      return doReplace(this, needle, replacement, count);
    }

    /*
      Splits the :record:`bytes` on `sep` yielding the subbytes between each
      occurrence, up to `maxsplit` times.

      :arg sep: The delimiter used to break the object into chunks.
      :arg maxsplit: The number of times to split the object, negative values
                     indicate no limit.
      :arg ignoreEmpty: * When `true`  -- Empty :record:`bytes` will not be
                                          yielded, and will not count towards
                                          `maxsplit`
                        * When `false` -- Empty :record:`bytes` will be yielded
                                          when `sep` occurs multiple times in a
                                          row.
     */
    iter split(sep: bytes, maxsplit: int = -1,
               ignoreEmpty: bool = false): bytes {
      for s in doSplit(this, sep, maxsplit, ignoreEmpty) do yield s;
    }

    /*
      Works as above, but uses runs of whitespace as the delimiter.

      :arg maxsplit: The number of times to split the bytes, negative values
                     indicate no limit.
     */
    iter split(maxsplit: int = -1) : bytes {
      if !this.isEmpty() {
        const localThis: bytes = this.localize();
        var done : bool = false;
        var yieldChunk : bool = false;
        var chunk : bytes;

        const noSplits : bool = maxsplit == 0;
        const limitSplits : bool = maxsplit > 0;
        var splitCount: int = 0;
        const iEnd: idxType = localThis.len - 1;

        var inChunk : bool = false;
        var chunkStart : idxType;

        for (i,c) in zip(1.., localThis.bytes()) {
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
      Returns a new :record:`bytes` object, which is the concatenation of all of
      the :record:`bytes` objects passed in with the contents of the method
      receiver inserted between them.
    */
    proc join(const ref S: bytes ...) : bytes {
      return _join(S);
    }

    /*
      Same as the varargs version, but with a homogeneous tuple of bytes.
    */
    proc join(const ref S) : bytes where isTuple(S) {
      if !isHomogeneousTuple(S) || !isBytes(S[1]) then
        compilerError("join() on tuples only handles homogeneous tuples of strings");
      return _join(S);
    }

    /*
      Same as the varargs version, but with all the bytes in an array.
     */
    proc join(const ref S: [] bytes) : bytes {
      return _join(S);
    }

    pragma "no doc"
    proc join(ir: _iteratorRecord): bytes {
      return doJoinIterator(this, ir);
    }

    pragma "no doc"
    proc _join(const ref S) : bytes where isTuple(S) || isArray(S) {
      return doJoin(this, S);
    }

    /*
      :arg chars: A :record:`bytes` containing each character to remove.
                  Defaults to `" \\t\\r\\n"`.
      :arg leading: Indicates if leading occurrences should be removed.
                    Defaults to `true`.
      :arg trailing: Indicates if trailing occurrences should be removed.
                     Defaults to `true`.

      :returns: A new :record:`bytes` with `leading` and/or `trailing`
                occurrences of characters in `chars` removed as appropriate.
    */
    proc strip(chars: bytes = " \t\r\n":bytes,
               leading=true, trailing=true) : bytes {
      if this.isEmpty() then return "";
      if chars.isEmpty() then return this;

      const localThis: bytes = this.localize();
      const localChars: bytes = chars.localize();

      var start: idxType = 1;
      var end: idxType = localThis.len;

      if leading {
        label outer for (i, thisChar) in zip(1.., localThis.bytes()) {
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
        label outer for (i, thisChar) in zip(1.., localThis.bytes()) {
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
      Splits the object on `sep` into a `3*bytes` consisting of the section
      before `sep`, `sep`, and the section after `sep`. If `sep` is not found,
      the tuple will contain the whole bytes, and then two empty bytes.
    */
    proc const partition(sep: bytes) : 3*bytes {
      return doPartition(this, sep);
    }

    /*
      Returns a UTF-8 string from the given :record:`bytes` object. If the data
      is malformed for UTF-8, `errors` argument determines the action.
      
      :arg errors: `DecodePolicy.Strict` raises an error, `DecodePolicy.Replace`
                   replaces the malformed character with UTF-8 replacement
                   character, `DecodePolicy.Ignore` drops the data silently.
      
      :throws: `DecodeError` if `DecodePolicy.Strict` is passed to the `errors`
               argument and the `bytes` object contains non-UTF-8 characters.

      :returns: A UTF-8 string.
    */
    // NOTE: In the future this could support more encodings.
    proc decode(errors=DecodePolicy.Strict): string throws {
      pragma "fn synchronization free"
      extern proc qio_decode_char_buf(ref chr:int(32), ref nbytes:c_int,
                                      buf:c_string, buflen:ssize_t):syserr;

      var localThis: bytes = this.localize();

      // allocate buffer the same size as this buffer assuming that the string
      // is in fact perfectly decodable. In the worst case, the user wants the
      // replacement policy and we grow the buffer couple of times.
      // The alternative is to allocate more space from the beginning.
      var ret: string;
      var (newBuff, allocSize) = bufferAlloc(this.len+1);
      ret.buff = newBuff;
      ret._size = allocSize;
      ret.isowned = true;

      var thisIdx = 0;
      var decodedIdx = 0;
      while thisIdx < localThis.len {
        var cp: int(32);
        var nbytes: c_int;
        var bufToDecode = (localThis.buff + thisIdx): c_string;
        var maxbytes = (localThis.len - thisIdx): ssize_t;
        qio_decode_char_buf(cp, nbytes, bufToDecode, maxbytes);

        if cp == 0xfffd {  //decoder returns the replacament character
          if errors == DecodePolicy.Strict {
            throw new owned DecodeError();
          }
          else if errors == DecodePolicy.Ignore {
            thisIdx += nbytes; //skip over the malformed bytes
            continue;
          }
        }

        // do a naive copy
        bufferMemcpyLocal(dst=ret.buff, src=bufToDecode, len=nbytes,
                          dst_off=decodedIdx);
        thisIdx += nbytes;
        decodedIdx += nbytes;
      }

      ret.len = decodedIdx;
      ret.buff[ret.len] = 0;
      return ret;
    }

    /*
     Checks if all the characters in the object are uppercase (A-Z) in ASCII.

      :returns: * `true`  -- if the object contains at least one uppercase
                             character and no lowercase characters, ignoring
                             uncased (not a letter) and extended ASCII
                             characters (decimal value is larger than 127)
                            
                * `false` -- otherwise
     */
    proc isUpper() : bool {
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
     Checks if all the characters in the object are lowercase (a-z) in ASCII.

      :returns: * `true`  -- if the object contains at least one lowercase
                             character and no upper characters, ignoring
                             uncased (not a letter) and extended ASCII
                             characters (decimal value is larger than 127)
                            
                * `false` -- otherwise
     */
    proc isLower() : bool {
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
     Checks if all the characters in the object are whitespace (' ', '\t',
     '\n', '\v', '\f', '\r') in ASCII.

      :returns: * `true`  -- when all the characters are whitespace.
                * `false` -- otherwise
     */
    proc isSpace() : bool {
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
     Checks if all the characters in the object are alphabetic (a-zA-Z) in
     ASCII.

      :returns: * `true`  -- when the characters are alphabetic.
                * `false` -- otherwise
     */
    proc isAlpha() : bool {
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
     Checks if all the characters in the object are digits (0-9) in ASCII.

      :returns: * `true`  -- when the characters are digits.
                * `false` -- otherwise
     */
    proc isDigit() : bool {
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
     Checks if all the characters in the object are alphanumeric (a-zA-Z0-9) in
     ASCII.

      :returns: * `true`  -- when the characters are alphanumeric.
                * `false` -- otherwise
     */
    proc isAlnum() : bool {
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
     Checks if all the characters in the object are printable in ASCII.

      :returns: * `true`  -- when the characters are printable.
                * `false` -- otherwise
     */
    proc isPrintable() : bool {
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
    proc isTitle() : bool {
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
      :returns: A new :record:`bytes` with all uppercase characters (A-Z)
                replaced with their lowercase counterpart in ASCII. Other
                characters remain untouched.
    */
    proc toLower() : bytes {
      var result: bytes = this;
      if result.isEmpty() then return result;
      for (i,b) in zip(0.., result.bytes()) {
        result.buff[i] = byte_toLower(b); //check is done by byte_toLower
      }
      return result;
    }

    /*
      :returns: A new bytes with all lowercase characters (a-z) replaced with
                their uppercase counterpart in ASCII. Other characters remain
                untouched.
    */
    proc toUpper() : bytes {
      var result: bytes = this;
      if result.isEmpty() then return result;
      for (i,b) in zip(0.., result.bytes()) {
        result.buff[i] = byte_toUpper(b); //check is done by byte_toUpper
      }
      return result;
    }

    /*
      :returns: A new bytes with all cased characters(a-zA-Z) following an
                uncased character converted to uppercase, and all cased
                characters following another cased character converted to
                lowercase.
     */
    proc toTitle() : bytes {
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

  } // end of record bytes

  //
  // createBytes* functions
  //

  inline proc createBytesWithBorrowedBuffer(s: bytes) {
    var ret: bytes;
    initWithBorrowedBuffer(ret, s);
    return ret;
  }

  inline proc createBytesWithBorrowedBuffer(s: c_string, length=s.length) {
    return createBytesWithBorrowedBuffer(s:c_ptr(uint(8)), length=length,
                                                            size=length+1);
  }

  inline proc createBytesWithBorrowedBuffer(s: bufferType, length: int, size: int) {
    var ret: bytes;
    initWithBorrowedBuffer(ret, s, length,size);
    return ret;
  }

  inline proc createBytesWithOwnedBuffer(s: bytes) {
    // should we allow stealing ownership?
    compilerError("A bytes cannot be passed to createBytesWithOwnedBuffer");
  }

  inline proc createBytesWithOwnedBuffer(s: c_string, length=s.length) {
    return createBytesWithOwnedBuffer(s: bufferType, length=length,
                                                      size=length+1);
  }

  inline proc createBytesWithOwnedBuffer(s: bufferType, length: int, size: int) {
    var ret: bytes;
    initWithOwnedBuffer(ret, s, length, size);
    return ret;
  }

  inline proc createBytesWithNewBuffer(s: bytes) {
    var ret: bytes;
    initWithNewBuffer(ret, s);
    return ret;
  }

  inline proc createBytesWithNewBuffer(s: c_string, length=s.length) {
    return createBytesWithNewBuffer(s: bufferType, length=length,
                                                    size=length+1);
  }

  inline proc createBytesWithNewBuffer(s: bufferType, length: int, size: int) {
    var ret: bytes;
    initWithNewBuffer(ret, s, length, size);
    return ret;
  }

  inline proc _cast(type t: bytes, x: string) {
    return createBytesWithNewBuffer(x.buff, length=x.numBytes, size=x.numBytes+1);
  }

  /*
     Appends the bytes `rhs` to the bytes `lhs`.
  */
  proc +=(ref lhs: bytes, const ref rhs: bytes) : void {
    doAppend(lhs, rhs);
  }

  /*
     Copies the bytes `rhs` into the bytes `lhs`.
  */
  proc =(ref lhs: bytes, rhs: bytes) {
    doAssign(lhs, rhs);
  }

  /*
     Copies the c_string `rhs_c` into the bytes `lhs`.

     Halts if `lhs` is a remote bytes.
  */
  proc =(ref lhs: bytes, rhs_c: c_string) {
    doAssign(lhs, rhs_c);
  }

  //
  // Concatenation
  //
  /*
     :returns: A new :record:`bytes` which is the result of concatenating `s0`
               and `s1`
  */
  proc +(s0: bytes, s1: bytes) {
    return doConcat(s0, s1);
  }

  /*
     :returns: A new :record:`bytes` which is the result of repeating `s` `n`
               times.  If `n` is less than or equal to 0, an empty bytes is
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
  proc ==(a: bytes, b: string) : bool {
    return doEq(a,b);
  }

  pragma "no doc"
  proc ==(a: string, b: bytes) : bool {
    return doEq(a,b);
  }

  pragma "no doc"
  inline proc !=(a: bytes, b: bytes) : bool {
    return !doEq(a,b);
  }

  pragma "no doc"
  inline proc !=(a: bytes, b: string) : bool {
    return !doEq(a,b);
  }

  pragma "no doc"
  inline proc !=(a: string, b: bytes) : bool {
    return !doEq(a,b);
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

} // end of module Bytes
