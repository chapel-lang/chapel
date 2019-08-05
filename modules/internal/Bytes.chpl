
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

  // Growth factor to use when extending the buffer for appends
  private config param chpl_stringGrowthFactor = 1.5;


  // Following is copy-paste from string
  //
  // Externs and constants used to implement strings
  //
  private        param chpl_string_min_alloc_size: int = 16;
  pragma "fn synchronization free"
  private extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;

  // Calls to chpl_here_alloc increment the memory descriptor by
  // `chpl_memhook_md_num`. For internal runtime descriptors like the ones
  // below, this would result in selecting the incorrect descriptor string.
  //
  // Instead, decrement the CHPL_RT_MD* descriptor and use the result when
  // calling chpl_here_alloc.
  private proc offset_STR_COPY_DATA {
    extern const CHPL_RT_MD_STR_COPY_DATA: chpl_mem_descInt_t;
    return CHPL_RT_MD_STR_COPY_DATA - chpl_memhook_md_num();
  }
  private proc offset_STR_COPY_REMOTE {
    extern const CHPL_RT_MD_STR_COPY_REMOTE: chpl_mem_descInt_t;
    return CHPL_RT_MD_STR_COPY_REMOTE - chpl_memhook_md_num();
  }
  pragma "fn synchronization free"
  private extern proc qio_decode_char_buf(ref chr:int(32), ref nbytes:c_int, buf:c_string, buflen:ssize_t):syserr;

  type idxType = int; 

  type byteType = uint(8);
  type bufferType = c_ptr(byteType);

  private inline proc chpl_string_comm_get(dest: bufferType, src_loc_id: int(64),
                                           src_addr: bufferType, len: integral) {
    __primitive("chpl_comm_get", dest, src_loc_id, src_addr, len.safeCast(size_t));
  }

  private proc copyRemoteBuffer(src_loc_id: int(64), src_addr: bufferType,
                                len: int): bufferType {
      const dest = chpl_here_alloc(len+1, offset_STR_COPY_REMOTE): bufferType;
      chpl_string_comm_get(dest, src_loc_id, src_addr, len);
      dest[len] = 0;
      return dest;
  }

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
      Initialize a new :record:`bytes` from ``b``. If ``isowned`` is set to
      ``true`` then ``b`` will be fully copied into the new instance. If it is
      ``false`` a shallow copy will be made such that any in-place modifications
      to the new bytes may appear in ``b``. It is the responsibility of the user
      to ensure that the underlying buffer is not freed while being used as part
      of a shallow copy.
     */
    /*proc init(b: bytes, isowned: bool = true) {*/

    /*}*/

    /*
      Initialize a new :record:`bytes` from ``s``. If ``isowned`` is set to
      ``true`` then ``b`` will be fully copied into the new instance. If it is
      ``false`` a shallow copy will be made such that any in-place modifications
      to the new bytes may appear in ``b``. It is the responsibility of the user
      to ensure that the underlying buffer is not freed while being used as part
      of a shallow copy.
     */
    proc init(s, isowned: bool = true) where s.type == string || s.type == _bytes {
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
          this.buff = copyRemoteBuffer(s.locale_id, s.buff, sLen);
          this._size = sLen+1;
        } else {
          if this.isowned {
            const allocSize = chpl_here_good_alloc_size(sLen+1);
            this.buff = chpl_here_alloc(allocSize,
                                       offset_STR_COPY_DATA): bufferType;
            c_memcpy(this.buff, s.buff, s.len);
            this.buff[sLen] = 0;
            this._size = allocSize;
          } else {
            this.buff = s.buff;
            this._size = s._size;
          }
        }
      }
    }


    // this is implemented only for debugging purposes. Ideally writeThis should
    // just halt when called on bytes record
    pragma "no doc"
    proc writeThis(f) {
      var localThis = this.localize();
      try {
        for b in localThis.iterBytes() {
          if byte_isAscii(b) {
            if byte_isPrintable(b) || byte_isWhitespace(b) {
              f.writef("%c", b);
            }
          }
        }
      } catch e: SystemError {
        f.setError(e.err);
      } catch {
        f.setError(EINVAL:syserr);
      }
    }

    pragma "no doc"
    proc init=(b: _bytes) {
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
    proc ref reinitString(_buf: c_ptr, s_len: int, size: int,
                          needToCopy:bool = true) {
      if this.isEmpty() && _buf == nil then return;

      const buf = _buf:bufferType; // this is different than string

      // If the this.buff is longer than buf, then reuse the buffer if we are
      // allowed to (this.isowned == true)
      if s_len != 0 {
        if needToCopy {
          if !this.isowned || s_len+1 > this._size {
            // If the new string is too big for our current buffer or we dont
            // own our current buffer then we need a new one.
            if this.isowned && !this.isEmpty() then
              chpl_here_free(this.buff);
            // TODO: should I just allocate 'size' bytes?
            const allocSize = chpl_here_good_alloc_size(s_len+1);
            this.buff = chpl_here_alloc(allocSize,
                                       offset_STR_COPY_DATA):bufferType;
            this._size = allocSize;
            // We just allocated a buffer, make sure to free it later
            this.isowned = true;
          }
          c_memmove(this.buff, buf, s_len);
          this.buff[s_len] = 0;
        } else {
          if this.isowned && !this.isEmpty() then
            chpl_here_free(this.buff);
          this.buff = buf;
          this._size = size;
        }
      } else {
        // If s_len is 0, 'buf' may still have been allocated. Regardless, we
        // need to free the old buffer if 'this' is isowned.
        if this.isowned && !this.isEmpty() then chpl_here_free(this.buff);
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
       Gets a version of the :record:`bytes` that is on the currently
       executing locale.

       :returns: A shallow copy if the :record:`bytes` is already on the
                 current locale, otherwise a deep copy is performed.
    */
    inline proc localize() : _bytes {
      if _local || this.locale_id == chpl_nodeID {
        return new _bytes(this, isowned=false);
      } else {
        const x:_bytes = this; // assignment makes it local
        return x;
      }
    }


    /*
      Get a `c_string` from a :record:`bytes`.
     */
    inline proc c_str(): c_string {
      inline proc _cast(type t:c_string, x:bufferType) {
        return __primitive("cast", t, x);
      }

      if _local == false && this.locale_id != chpl_nodeID then
        halt("Cannot call .c_str() on a remote bytes");

      return this.buff:c_string;
    }

    /*
      Gets a byte from the object

      :arg i: The index

      :returns: 1-length :record:`bytes` object
     */
    proc this(i: int): _bytes {
      if boundsChecking && (i <= 0 || i > this.len)
        then halt("index out of bounds of bytes: ", i);
      return new _bytes(c_ptrTo(this.buff[i-1]), length=1, size=2);
    }

    /*
      :returns: The value of the `i` th byte as an integer.
    */
    proc getByte(i: int): byteType {
      if boundsChecking && (i <= 0 || i > this.len)
        then halt("index out of bounds of bytes: ", i);
      return this.buff[i-1];
    }

    /*
      Iterates over the bytes

      :yields: 1-length :record:`bytes` objects
     */
    iter these(): _bytes {
      if this.isEmpty() then return;
      for i in 1..this.len do
        yield this[i];
    }

    /*
      Iterates over the bytes byte by byte.
    */
    iter iterBytes(): byteType {
      for i in 1..this.len do
        yield this.getByte(i);
    }

    /*
      Slices the object. Halts if r is not completely inside the range
      ``1..bytes.length`` when compiled with `--checks`. `--fast` disables this
      check.

      :arg r: range of the indices the new bytes should be made from

      :returns: a new bytes that is a slice within ``1..bytes.length``. If
                the length of `r` is zero, an empty bytes is returned.
     */
    proc this(r: range(?)) : _bytes {
      var ret: _bytes;
      if this.isEmpty() then return ret;

      const r2 = this._getView(r);
      if r2.size <= 0 {
        // TODO: I can't just return "" (ret var gets freed for some reason)
        ret = "";
      } else {
        ret.len = r2.size:int;
        const newSize = chpl_here_good_alloc_size(ret.len+1);
        ret._size = max(chpl_string_min_alloc_size, newSize);
        // FIXME: I was dumb here, just copy the correct region over in
        // multi-locale and use that as the string buffer. No need to copy stuff
        // about after pulling it across.
        ret.buff = chpl_here_alloc(ret._size,
                                  offset_STR_COPY_DATA): bufferType;

        var thisBuff: bufferType;
        const remoteThis = _local == false && this.locale_id != chpl_nodeID;
        if remoteThis {
          // TODO: Could do an optimization here and only pull down the data
          // between r2.low and r2.high. Indexing for the copy below gets a bit
          // more complex when that is performed though.
          thisBuff = copyRemoteBuffer(this.locale_id, this.buff, this.len);
        } else {
          thisBuff = this.buff;
        }

        var buff = ret.buff; // Has perf impact and our LICM can't hoist :(
        for (r2_i, i) in zip(r2, 0..) {
          buff[i] = thisBuff[r2_i-1];
        }
        buff[ret.len] = 0;

        if remoteThis then chpl_here_free(thisBuff);
      }

      return ret;

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
    proc startsWith(needles: _bytes ...) : bool {
      return _startsEndsWith((...needles), fromLeft=true);
    }

    /*
      :arg needles: A varargs list of :record:`bytes` objects to match against.

      :returns: * `true`  -- when the object ends with one or more of the `needles`
                * `false` -- otherwise
     */
    proc endsWith(needles: _bytes ...) : bool {
      return _startsEndsWith((...needles), fromLeft=false);

    }

    // TODO: could use a multi-pattern search or some variant when there are
    // multiple needles. Probably wouldn't be worth the overhead for small
    // needles though
    pragma "no doc"
    inline proc _startsEndsWith(needles: _bytes ..., param fromLeft: bool) : bool {
      var ret: bool = false;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for needle in needles {
          if needle.isEmpty() {
            ret = true;
            break;
          }
          if needle.len > this.len then continue;

          const localNeedle: _bytes = needle.localize();

          const needleR = 0:int..#localNeedle.len;
          if fromLeft {
            const result = c_memcmp(this.buff, localNeedle.buff,
                                    localNeedle.len);
            ret = result == 0;
          } else {
            var offset = this.len-localNeedle.len;
            const result = c_memcmp(this.buff+offset, localNeedle.buff,
                                    localNeedle.len);
            ret = result == 0;
          }
          if ret == true then break;
        }
      }
      return ret;
    }

    /*
      :arg needle: the :record:`bytes` object to search for
      :arg region: an optional range defining the indices to search within,
                   default is the whole object. Halts if the range is not
                   within ``1..bytes.length``

      :returns: the index of the first occurrence of `needle` within a
                the object, or 0 if the `needle` is not in the object.
     */
    proc find(needle: _bytes, region: range(?) = 1:idxType..) : idxType {
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
    proc rfind(needle: _bytes, region: range(?) = 1:idxType..) : idxType {
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
    proc count(needle: _bytes, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=true);
    }

    // Helper function that uses a param bool to toggle between count and find
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    //
    pragma "no doc"
    inline proc _search_helper(needle: _bytes, region: range(?),
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
            localRet = thisLen+1;
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
          const localNeedle: _bytes = needle.localize();

          // i *is not* an index into anything, it is the order of the element
          // of view we are searching from.
          const numPossible = thisLen - nLen + 1;
          const searchSpace = if fromLeft
              then 0..#(numPossible)
              else 0..#(numPossible) by -1;
          for i in searchSpace {
            // j *is* the index into the localNeedle's buffer
            for j in 0..#nLen {
              const idx = view.orderToIndex(i+j); // 1s based idx
              if this.buff[idx-1] != localNeedle.buff[j] then break;

              if j == nLen-1 {
                if count {
                  localRet += 1;
                } else { // find
                  localRet = view.orderToIndex(i);
                }
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
    proc replace(needle: _bytes, replacement: _bytes, count: int = -1) : _bytes {
      var result: _bytes = this;
      var found: int = 0;
      var startIdx: idxType = 1;
      const localNeedle: _bytes = needle.localize();
      const localReplacement: _bytes = replacement.localize();

      while (count < 0) || (found < count) {
        const idx = result.find(localNeedle, startIdx..);
        if !idx then break;

        found += 1;
        result = result[..idx-1] + localReplacement +
                 result[(idx + localNeedle.length)..];
        var tmp_res = result[..idx-1] + localReplacement +
                 result[(idx + localNeedle.length)..];

        startIdx = idx + localReplacement.length;
      }
      return result;

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
    iter split(sep: _bytes, maxsplit: int = -1,
               ignoreEmpty: bool = false): _bytes {
      if !(maxsplit == 0 && ignoreEmpty && this.isEmpty()) {
        const localThis: _bytes = this.localize();
        const localSep: _bytes = sep.localize();

        // really should be <, but we need to avoid returns and extra yields so
        // the iterator gets inlined
        var splitAll: bool = maxsplit <= 0;
        var splitCount: int = 0;

        var start: idxType = 1;
        var done: bool = false;
        while !done  {
          var chunk: _bytes;
          var end: idxType;

          if (maxsplit == 0) {
            chunk = localThis;
            done = true;
          } else {
            if (splitAll || splitCount < maxsplit) then
              end = localThis.find(localSep, start..);

            if(end == 0) {
              // Separator not found
              chunk = localThis[start..];
              done = true;
            } else {
              chunk = localThis[start..end-1];
            }
          }

          if !(ignoreEmpty && chunk.isEmpty()) {
            // Putting the yield inside the if prevents us from being inlined
            // in the zippered case, but I don't think there is any way to avoid
            // that easily
            yield chunk;
            splitCount += 1;
          }
          start = end+localSep.length;
        }
      }

    }

    /*
      Works as above, but uses runs of whitespace as the delimiter.

      :arg maxsplit: The number of times to split the bytes, negative values
                     indicate no limit.
     */
    iter split(maxsplit: int = -1) : _bytes {
      if !this.isEmpty() {
        const localThis: _bytes = this.localize();
        var done : bool = false;
        var yieldChunk : bool = false;
        var chunk : _bytes;

        const noSplits : bool = maxsplit == 0;
        const limitSplits : bool = maxsplit > 0;
        var splitCount: int = 0;
        const iEnd: idxType = localThis.len - 1;

        var inChunk : bool = false;
        var chunkStart : idxType;

        for (i,c) in zip(1.., localThis.iterBytes()) {
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
    proc join(const ref S: _bytes ...) : _bytes {
      return _join(S);
    }

    /*
      Same as the varargs version, but with a homogeneous tuple of bytes.
    */
    proc join(const ref S) : _bytes where isTuple(S) {
      if !isHomogeneousTuple(S) || !isBytes(S[1]) then
        compilerError("join() on tuples only handles homogeneous tuples of strings");
      return _join(S);
    }

    /*
      Same as the varargs version, but with all the bytes in an array.
     */
    proc join(const ref S: [] _bytes) : _bytes {
      return _join(S);
    }

    pragma "no doc"
    proc join(ir: _iteratorRecord) {
      var s: _bytes;
      var first: bool = true;
      for i in ir {
        if first then
          first = false;
        else
          s += this;
        s += i;
      }
      return s;

    }

    pragma "no doc"
    proc _join(const ref S) : _bytes where isTuple(S) || isArray(S) {
      if S.size == 0 {
        return '';
      } else if S.size == 1 {
        // TODO: ensures copy, clean up when no longer needed
        var ret: _bytes;
        if (isArray(S)) {
          ret = S[S.domain.first];
        } else {
          ret = S[1];
        }
        return ret;
      } else {
        var joinedSize: int = this.len * (S.size - 1);
        for s in S do joinedSize += s.length;

        if joinedSize == 0 then
          return '';

        var joined: _bytes;
        joined.len = joinedSize;
        const allocSize = chpl_here_good_alloc_size(joined.len + 1);
        joined._size = allocSize;
        joined.buff = chpl_here_alloc(
          allocSize,
          offset_STR_COPY_DATA): bufferType;

        var first = true;
        var offset = 0;
        for s in S {
          if first {
            first = false;
          } else if this.len != 0 {
            c_memcpy(joined.buff + offset, this.buff, this.len);
            offset += this.len;
          }

          var sLen = s.len;
          if sLen != 0 {
            var cpyStart = joined.buff + offset;
            if _local || s.locale_id == chpl_nodeID {
              c_memcpy(cpyStart, s.buff, sLen);
            } else {
              chpl_string_comm_get(cpyStart, s.locale_id, s.buff, sLen);
            }
            offset += sLen;
          }
        }
        joined.buff[offset] = 0;
        return joined;
      }
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
    proc strip(chars: _bytes = " \t\r\n":_bytes, leading=true, trailing=true) : _bytes {
      if this.isEmpty() then return "";
      if chars.isEmpty() then return this;

      const localThis: _bytes = this.localize();
      const localChars: _bytes = chars.localize();

      var start: idxType = 1;
      var end: idxType = localThis.len;

      if leading {
        label outer for (i, thisChar) in zip(1.., localThis.iterBytes()) {
          for removeChar in localChars.iterBytes() {
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
        label outer for (i, thisChar) in zip(1.., localThis.iterBytes()) {
          for removeChar in localChars.iterBytes() {
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
    proc const partition(sep: _bytes) : 3*_bytes {
      const idx = this.find(sep);
      if idx != 0 {
        return (this[..idx-1], sep, this[idx+sep.length..]);
      } else {
        return (this, "":_bytes, "":_bytes);
      }
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
      var localThis: _bytes = this.localize();

      // allocate buffer the same size as this buffer assuming that the string
      // is in fact perfectly decodable. In the worst case, the user wants the
      // replacement policy and we grow the buffer couple of times.
      // The alternative is to allocate more space from the beginning.
      var allocSize = chpl_here_good_alloc_size(this.len+1);
      var c_buf = chpl_here_alloc(allocSize, offset_STR_COPY_DATA): bufferType;

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
        c_memcpy(c_ptrTo(c_buf[decodedIdx]), bufToDecode:c_void_ptr, nbytes);
        thisIdx += nbytes;
        decodedIdx += nbytes;
      }

      // I couldn't use any named args here, why?
      return new string(c_buf, decodedIdx, allocSize, true, false);
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
        for b in this.iterBytes() {
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
        for b in this.iterBytes() {
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
        for b in this.iterBytes() {
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
        for b in this.iterBytes() {
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
        for b in this.iterBytes() {
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
        for b in this.iterBytes() {
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
        for b in this.iterBytes() {
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
        for b in this.iterBytes() {
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
    proc toLower() : _bytes {
      var result: _bytes = this;
      if result.isEmpty() then return result;
      for (i,b) in zip(0.., result.iterBytes()) {
        result.buff[i] = byte_toLower(b); //check is done by byte_toLower
      }
      return result;
    }

    /*
      :returns: A new bytes with all lowercase characters (a-z) replaced with
                their uppercase counterpart in ASCII. Other characters remain
                untouched.
    */
    proc toUpper() : _bytes {
      var result: _bytes = this;
      if result.isEmpty() then return result;
      for (i,b) in zip(0.., result.iterBytes()) {
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
    proc toTitle() : _bytes {
      var result: _bytes = this;
      if result.isEmpty() then return result;

      param UN = 0, LETTER = 1;
      var last = UN;
      for (i,b) in zip(0.., result.iterBytes()) {
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

  inline proc _cast(type t: _bytes, x: string) {
    return new _bytes(x);
  }

  /*
     Appends the bytes `rhs` to the bytes `lhs`.
  */
  proc +=(ref lhs: _bytes, const ref rhs: _bytes) : void {
    // if rhs is empty, nothing to do
    if rhs.len == 0 then return;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
      const rhsLen = rhs.len;
      const newLength = lhs.len+rhsLen; //TODO: check for overflow
      if lhs._size <= newLength {
        const newSize = chpl_here_good_alloc_size(
            max(newLength+1, lhs.len*chpl_stringGrowthFactor):int);

        if lhs.isowned {
          lhs.buff = chpl_here_realloc(lhs.buff, newSize,
                                      offset_STR_COPY_DATA):bufferType;
        } else {
          var newBuff = chpl_here_alloc(newSize,
                                       offset_STR_COPY_DATA):bufferType;
          c_memcpy(newBuff, lhs.buff, lhs.len);
          lhs.buff = newBuff;
          lhs.isowned = true;
        }

        lhs._size = newSize;
      }
      const rhsRemote = rhs.locale_id != chpl_nodeID;
      if rhsRemote {
        chpl_string_comm_get(lhs.buff+lhs.len, rhs.locale_id, rhs.buff, rhsLen);
      } else {
        c_memcpy(lhs.buff+lhs.len, rhs.buff, rhsLen);
      }
      lhs.len = newLength;
      lhs.buff[newLength] = 0;
    }
  }

  /*
     Copies the bytes `rhs` into the bytes `lhs`.
  */
  proc =(ref lhs: _bytes, rhs: _bytes) {
    inline proc helpMe(ref lhs: _bytes, rhs: _bytes) {
      if _local || rhs.locale_id == chpl_nodeID {
        lhs.reinitString(rhs.buff, rhs.len, rhs._size, needToCopy=true);
      } else {
        const len = rhs.len; // cache the remote copy of len
        var remote_buf:bufferType = nil;
        if len != 0 then
          remote_buf = copyRemoteBuffer(rhs.locale_id, rhs.buff, len);
        lhs.reinitString(remote_buf, len, len+1, needToCopy=false);
      }
    }

    if _local || lhs.locale_id == chpl_nodeID then {
      helpMe(lhs, rhs);
    }
    else {
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
        helpMe(lhs, rhs);
      }
    }

  }

  /*
     Copies the c_string `rhs_c` into the bytes `lhs`.

     Halts if `lhs` is a remote bytes.
  */
  proc =(ref lhs: _bytes, rhs_c: c_string) {
    // Make this some sort of local check once we have local types/vars
    if !_local && (lhs.locale_id != chpl_nodeID) then
      halt("Cannot assign a c_string to a remote string.");

    const len = rhs_c.length;
    const buff:bufferType = rhs_c:bufferType;
    lhs.reinitString(buff, len, len+1, needToCopy=true);
  }

  //
  // Concatenation
  //
  /*
     :returns: A new :record:`bytes` which is the result of concatenating `s0`
               and `s1`
  */
  proc +(s0: _bytes, s1: _bytes) {
    return _bytes_string_concat(s0,s1);
  }

  proc +(s0: _bytes, s1: string) {
    return _bytes_string_concat(s0,s1);
  }

  proc +(s0: string, s1: _bytes) {
    return _bytes_string_concat(s0,s1);
  }

  private proc _bytes_string_concat(s0: ?t, s1): t {
    // cache lengths locally
    const s0len = s0.len;
    if s0len == 0 then return s1:t;
    const s1len = s1.len;
    if s1len == 0 then return s0;

    var ret: t;
    ret.len = s0len + s1len;
    const allocSize = chpl_here_good_alloc_size(ret.len+1);
    ret._size = allocSize;
    ret.buff = chpl_here_alloc(allocSize,
                              offset_STR_COPY_DATA): bufferType;
    ret.isowned = true;

    const s0remote = s0.locale_id != chpl_nodeID;
    if s0remote {
      chpl_string_comm_get(ret.buff, s0.locale_id, s0.buff, s0len);
    } else {
      c_memcpy(ret.buff, s0.buff, s0len);
    }

    const s1remote = s1.locale_id != chpl_nodeID;
    if s1remote {
      chpl_string_comm_get(ret.buff+s0len, s1.locale_id, s1.buff, s1len);
    } else {
      c_memcpy(ret.buff+s0len, s1.buff, s1len);
    }
    ret.buff[ret.len] = 0;
    return ret;

  }

  /*
     :returns: A new :record:`bytes` which is the result of repeating `s` `n`
               times.  If `n` is less than or equal to 0, an empty bytes is
               returned.
  */
  proc *(s: _bytes, n: integral) {
    if n <= 0 then return new _bytes("");

    const sLen = s.length;
    if sLen == 0 then return new _bytes("");

    var ret: _bytes;
    ret.len = sLen * n; // TODO: check for overflow
    const allocSize = chpl_here_good_alloc_size(ret.len+1);
    ret._size = allocSize;
    ret.buff = chpl_here_alloc(allocSize,
                              offset_STR_COPY_DATA): bufferType;
    ret.isowned = true;

    const sRemote = s.locale_id != chpl_nodeID;
    if sRemote {
      chpl_string_comm_get(ret.buff, s.locale_id, s.buff, sLen);
    } else {
      c_memcpy(ret.buff, s.buff, sLen);
    }

    var iterations = n-1;
    var offset = sLen;
    for i in 1..iterations {
      c_memcpy(ret.buff+offset, ret.buff, sLen);
      offset += sLen;
    }
    ret.buff[ret.len] = 0;

    return ret;

  }

  // Concatenation with other types is done by casting to bytes
  private inline proc concatHelp(s: _bytes, x:?t) where t != _bytes {
    var cs = x:_bytes;
    const ret = s + cs;
    return ret;
  }

  private inline proc concatHelp(x:?t, s: _bytes) where t != _bytes  {
    var cs = x:_bytes;
    const ret = cs + s;
    return ret;
  }

  /*
     The following concatenation functions return a new :record:`bytes` which is
     the result of casting the non-bytes argument to a bytes, and concatenating
     that result with `s`.
  */
  inline proc +(s: _bytes, x: numeric) return concatHelp(s, x);
  inline proc +(x: numeric, s: _bytes) return concatHelp(x, s);
  inline proc +(s: _bytes, x: enumerated) return concatHelp(s, x);
  inline proc +(x: enumerated, s: _bytes) return concatHelp(x, s);
  inline proc +(s: _bytes, x: bool) return concatHelp(s, x);
  inline proc +(x: bool, s: _bytes) return concatHelp(x, s);

  // Relational operators
  private inline proc _strcmp_local(a, b) : int {
    // Assumes a and b are on same locale and not empty.
    const size = min(a.len, b.len);
    const result =  c_memcmp(a.buff, b.buff, size);

    if (result == 0) {
      // Handle cases where one string is the beginning of the other
      if (size < a.len) then return 1;
      if (size < b.len) then return -1;
    }
    return result;
  }

  private inline proc _strcmp(a, b) where a.type==_bytes||a.type==string &&
                                          b.type==_bytes||b.type==string : int {
    if a.locale_id == chpl_nodeID && b.locale_id == chpl_nodeID {
      // it's local
      return _strcmp_local(a, b);
    } else {
      var localA: _bytes = a.localize();
      var localB: _bytes = b.localize();
      return _strcmp_local(localA, localB);
    }
  }

  pragma "no doc"
  proc ==(a: _bytes, b: _bytes) : bool {
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

    return _strcmp(a, b) == 0;
  }

  pragma "no doc"
  proc ==(a: _bytes, b: string) : bool {
    return _strcmp(a, b) == 0;
  }

  pragma "no doc"
  proc ==(a: string, b: _bytes) : bool {
    return _strcmp(a, b) == 0;
  }

  pragma "no doc"
  inline proc !=(a: _bytes, b: _bytes) : bool {
    return _strcmp(a, b) != 0;
  }

  pragma "no doc"
  inline proc !=(a: _bytes, b: string) : bool {
    return _strcmp(a, b) != 0;
  }

  pragma "no doc"
  inline proc !=(a: string, b: _bytes) : bool {
    return _strcmp(a, b) != 0;
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
