/*
 * Copyright 2004-2018 Cray Inc.
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

  Besides the functions below, some other modules proved routines that are
  useful for working with strings. The :mod:`IO` module provides
  :proc:`IO.string.format` which creates a string that is the result of
  formatting. It also includes functions for reading and writing strings.
  The :mod:`Regexp` module also provides some routines for searching
  within strings.

  .. warning::

    While :record:`string` is intended to be a Unicode string, there is much
    left to do. As of Chapel 1.13, only ASCII strings can be expected to work
    correctly with all functions.

    Future work involves support for both ASCII and unicode strings, and
    allowing users to specify the encoding for individual strings.
 */
module String {
  use ChapelStandard;
  use CString;
  use SysCTypes;
  use StringCasts;

  // Growth factor to use when extending the buffer for appends
  private config param chpl_stringGrowthFactor = 1.5;

  //
  // Externs and constants used to implement strings
  //

  private        param chpl_string_min_alloc_size: int = 16;

  // TODO (EJR: 02/25/16): see if we can remove this explicit type declaration.
  // chpl_mem_descInt_t is really a well known compiler type since the compiler
  // emits calls for the chpl_mem_descs table. Maybe the compiler should just
  // create the type and export it to the runtime?
  pragma "no doc"
  extern type chpl_mem_descInt_t = int(16);

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

  pragma "no doc"
  type bufferType = c_ptr(uint(8));

  pragma "no doc"
  extern const CHPL_SHORT_STRING_SIZE : c_int;

  pragma "no doc"
  extern record chpl__inPlaceBuffer {};

  pragma "no doc"
  extern proc chpl__getInPlaceBufferData(const ref data : chpl__inPlaceBuffer) : c_ptr(uint(8));

  private inline proc chpl_string_comm_get(dest: bufferType, src_loc_id: int(64),
                                           src_addr: bufferType, len: integral) {
    __primitive("chpl_comm_get", dest, src_loc_id, src_addr, len.safeCast(size_t));
  }

  private proc copyRemoteBuffer(src_loc_id: int(64), src_addr: bufferType, len: int): bufferType {
      const dest = chpl_here_alloc(len+1, offset_STR_COPY_REMOTE): bufferType;
      chpl_string_comm_get(dest, src_loc_id, src_addr, len);
      dest[len] = 0;
      return dest;
  }

  private config param debugStrings = false;

  pragma "no doc"
  record __serializeHelper {
    var len       : int;
    var buff      : bufferType;
    var size      : int;
    var locale_id : chpl_nodeID.type;
    var shortData : chpl__inPlaceBuffer;
  }

  //
  // String Implementation
  //
  // TODO: We should be able to remove "ignore noinit", but doing so causes
  // memory leaks in various places. Investigate why later and add noinit back
  // in when possible.
  pragma "ignore noinit"
  pragma "no default functions" // avoid the default (read|write)This routines
  record string {
    pragma "no doc"
    var len: int = 0; // length of string in bytes
    pragma "no doc"
    var _size: int = 0; // size of the buffer we own
    pragma "no doc"
    var buff: bufferType = nil;
    pragma "no doc"
    var owned: bool = true;
    pragma "no doc"
    // We use chpl_nodeID as a shortcut to get at here.id without actually constructing
    // a locale object. Used when determining if we should make a remote transfer.
    var locale_id = chpl_nodeID; // : chpl_nodeID_t

    /*
      Construct a new string from ``s``. If ``owned`` is set to ``true`` then
      ``s`` will be fully copied into the new instance. If it is ``false`` a
      shallow copy will be made such that any in-place modifications to the new
      string may appear in ``s``. It is the responsibility of the user to
      ensure that the underlying buffer is not freed while being used as part
      of a shallow copy.
     */
    proc string(s: string, owned: bool = true) {
      const sRemote = s.locale_id != chpl_nodeID;
      const sLen = s.len;
      this.owned = owned;
      // Don't need to do anything if s is an empty string
      if sLen != 0 {
        this.len = sLen;
        if !_local && sRemote {
          // ignore supplied value of owned for remote strings so we don't leak
          this.owned = true;
          this.buff = copyRemoteBuffer(s.locale_id, s.buff, sLen);
          this._size = sLen+1;
        } else {
          if this.owned {
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

    /*
      Construct a new string from the `c_string` `cs`. If `owned` is set to
      true, the backing buffer will be freed when the new record is destroyed.
      If `needToCopy` is set to true, the `c_string` will be copied into the
      record, otherwise it will be used directly. It is the responsibility of
      the user to ensure that the underlying buffer is not freed if the
      `c_string` is not copied in.
     */
    proc string(cs: c_string, length: int = cs.length,
                owned: bool = true, needToCopy:  bool = true) {
      this.owned = owned;
      const cs_len = length;
      this.reinitString(cs:bufferType, cs_len, cs_len+1, needToCopy);
    }

    /*
      Construct a new string from `buff` ( `c_ptr(uint(8))` ). `size` indicates
      the total size of the buffer available, while `len` indicates the current
      length of the string in the buffer (the common case would be `size-1` for
      a C-style string). If `owned` is set to true, the backing buffer will be
      freed when the new record is destroyed. If `needToCopy` is set to true,
      the `c_string` will be copied into the record, otherwise it will be used
      directly. It is the responsibility of the user to ensure that the
      underlying buffer is not freed if the `c_string` is not copied in.
     */
    // This constructor can cause a leak if owned = false and needToCopy = true
    proc string(buff: bufferType, length: int, size: int,
                owned: bool = true, needToCopy: bool = true) {
      this.owned = owned;
      this.reinitString(buff, length, size, needToCopy);
    }

    pragma "no doc"
    proc ref deinit() {
      // Checking for size here isn't sufficient. A string may have been
      // initialized from a c_string allocated from memory but beginning with
      // a null-terminator.
      if owned && this.buff != nil {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
          chpl_here_free(this.buff);
        }
      }
    }

    pragma "no doc"
    proc chpl__serialize() {
      var data : chpl__inPlaceBuffer;
      if len <= CHPL_SHORT_STRING_SIZE {
        chpl_string_comm_get(chpl__getInPlaceBufferData(data), locale_id, buff, len);
      }
      return new __serializeHelper(len, buff, _size, locale_id, data);
    }

    pragma "no doc"
    proc type chpl__deserialize(data) {
      if data.locale_id != chpl_nodeID {
        if data.len <= CHPL_SHORT_STRING_SIZE {
          return new string(chpl__getInPlaceBufferData(data.shortData), data.len,
                            data.size, owned=true, needToCopy=true);
        } else {
          var localBuff = copyRemoteBuffer(data.locale_id, data.buff, data.len);
          return new string(localBuff, data.len, data.size,
                            owned=true, needToCopy=false);
        }
      } else {
        return new string(data.buff, data.len, data.size, owned = false, needToCopy=false);
      }
    }

    // This is assumed to be called from this.locale
    pragma "no doc"
    proc ref reinitString(buf: bufferType, s_len: int, size: int,
                          needToCopy:bool = true) {
      if this.isEmptyString() && buf == nil then return;

      // If the this.buff is longer than buf, then reuse the buffer if we are
      // allowed to (this.owned == true)
      if s_len != 0 {
        if needToCopy {
          if !this.owned || s_len+1 > this._size {
            // If the new string is too big for our current buffer or we dont
            // own our current buffer then we need a new one.
            if this.owned && !this.isEmptyString() then
              chpl_here_free(this.buff);
            // TODO: should I just allocate 'size' bytes?
            const allocSize = chpl_here_good_alloc_size(s_len+1);
            this.buff = chpl_here_alloc(allocSize,
                                       offset_STR_COPY_DATA):bufferType;
            this._size = allocSize;
            // We just allocated a buffer, make sure to free it later
            this.owned = true;
          }
          c_memmove(this.buff, buf, s_len);
          this.buff[s_len] = 0;
        } else {
          if this.owned && !this.isEmptyString() then
            chpl_here_free(this.buff);
          this.buff = buf;
          this._size = size;
        }
      } else {
        // If s_len is 0, 'buf' may still have been allocated. Regardless, we
        // need to free the old buffer if 'this' is owned.
        if this.owned && !this.isEmptyString() then chpl_here_free(this.buff);
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
      :returns: The number of characters in the string.
      */
    inline proc length return len;

    /*
       Gets a version of the :record:`string` that is on the currently
       executing locale.

       :returns: A shallow copy if the :record:`string` is already on the
                 current locale, otherwise a deep copy is performed.
    */
    inline proc localize() : string {
      if _local || this.locale_id == chpl_nodeID {
        return new string(this, owned=false);
      } else {
        const x:string = this; // assignment makes it local
        return x;
      }
    }

    /*
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
     */
    inline proc c_str(): c_string {
      inline proc _cast(type t, x) where t:c_string && x.type:bufferType {
        return __primitive("cast", t, x);
      }

      if this.locale_id != chpl_nodeID then
        halt("Cannot call .c_str() on a remote string");

      return this.buff:c_string;
    }

    pragma "no doc"
    inline proc param c_str() param : c_string {
      inline proc _cast(type t, x) where t:c_string && x.type:string {
        return __primitive("cast", t, x);
      }
      return this:c_string; // folded out in resolution
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
    iter these() : string {
      for i in 1..this.len {
        // This is pretty painful from a performance perspective right now,
        // allocates w/ every yield
        yield this[i];
      }
    }

    /*
      Index into a string

      :returns: A string with the character at the specified index from
                `1..string.length`
     */
    proc this(i: int) : string {
      if boundsChecking && (i <= 0 || i > this.len)
        then halt("index out of bounds of string: ", i);

      var ret: string;
      const newSize = chpl_here_good_alloc_size(2);
      ret._size = max(chpl_string_min_alloc_size, newSize);
      ret.len = 1;
      ret.buff = chpl_here_alloc(ret._size,
                                offset_STR_COPY_DATA): bufferType;
      ret.owned = true;

      const remoteThis = this.locale_id != chpl_nodeID;
      if remoteThis {
        chpl_string_comm_get(ret.buff, this.locale_id, this.buff + i - 1, 1);
      } else {
        ret.buff[0] = this.buff[i-1];
      }
      ret.buff[1] = 0;

      return ret;
    }

    // Checks to see if r is inside the bounds of this and returns a finite
    // range that can be used to iterate over a section of the string
    // TODO: move into the public interface in some form? better name if so?
    pragma "no doc"
    proc _getView(r:range(?)) {
      //TODO: halt()s should use string.writef at some point.
      if boundsChecking {
        if r.hasLowBound() {
          if r.low <= 0 then
            halt("range out of bounds of string");
            //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
        }
        if r.hasHighBound() {
          if (r.high < 0) || (r.high:int > this.len) then
            halt("range out of bounds of string");
            //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
        }
      }
      const ret = r[1:r.idxType..#(this.len:r.idxType)];
      return ret;
    }

    /*
      Slice a string. Halts if r is not completely inside the range
      `1..string.length`.

      :arg r: range of the indices the new string should be made from

      :returns: a new string that is a substring within `1..string.length`. If
                the length of `r` is zero, an empty string is returned.
     */
    // TODO: I wasn't very good about caching variables locally in this one.
    proc this(r: range(?)) : string {
      var ret: string;
      if this.isEmptyString() then return ret;

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
        const remoteThis = this.locale_id != chpl_nodeID;
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

    pragma "no doc"
    inline proc substring(i: int) {
      compilerError("substring removed: use string[index]");
    }

    pragma "no doc"
    inline proc substring(r: range) {
      compilerError("substring removed: use string[range]");
    }

    /*
      :returns: * `true`  -- when the string is empty
                * `false` -- otherwise
     */
    inline proc isEmptyString() : bool {
      return this.len == 0; // this should be enough of a check
    }

    // These should never be called (but are default functions for records)
    pragma "no doc"
    proc writeThis(f) {
      compilerError("not implemented: writeThis");
    }
    pragma "no doc"
    proc readThis(f) {
      compilerError("not implemented: readThis");
    }

    // TODO: could use a multi-pattern search or some variant when there are
    // multiple needles. Probably wouldn't be worth the overhead for small
    // needles though
    pragma "no doc"
    inline proc _startsEndsWith(needles: string ..., param fromLeft: bool) : bool {
      var ret: bool = false;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for needle in needles {
          if needle.isEmptyString() {
            ret = true;
            break;
          }
          if needle.len > this.len then continue;

          const localNeedle: string = needle.localize();

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
      :arg needles: A varargs list of strings to match against.

      :returns: * `true`  -- when the string begins with one or more of the `needles`
                * `false` -- otherwise
     */
    proc startsWith(needles: string ...) : bool {
      return _startsEndsWith((...needles), fromLeft=true);
    }

    /*
      :arg needles: A varargs list of strings to match against.

      :returns: * `true`  -- when the string ends with one or more of the `needles`
                * `false` -- otherwise
     */
    proc endsWith(needles: string ...) : bool {
      return _startsEndsWith((...needles), fromLeft=false);
    }


    // Helper function that uses a param bool to toggle between count and find
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    //
    pragma "no doc"
    inline proc _search_helper(needle: string, region: range(?),
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
          if nLen == 0 then
            localRet = thisLen+1;
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
          const localNeedle: string = needle.localize();

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
      :arg needle: the string to search for
      :arg region: an optional range defining the substring to search within,
                   default is the whole string. Halts if the range is not
                   within `1..string.length`

      :returns: the index of the first occurrence of `needle` within a
                string, or 0 if the `needle` is not in the string.
     */
    // TODO: better name than region?
    proc find(needle: string, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=false);
    }

    /*
      :arg needle: the string to search for
      :arg region: an optional range defining the substring to search within,
                   default is the whole string. Halts if the range is not
                   within `1..string.length`

      :returns: the index of the first occurrence from the right of `needle`
                within a string, or 0 if the `needle` is not in the string.
     */
    proc rfind(needle: string, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=false, fromLeft=false);
    }

    /*
      :arg needle: the string to search for
      :arg region: an optional range defining the substring to search within,
                   default is the whole string. Halts if the range is not
                   within `1..string.length`

      :returns: the number of times `needle` occurs in the string
     */
    proc count(needle: string, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=true);
    }

    /*
      :arg needle: the string to search for
      :arg replacement: the string to replace `needle` with
      :arg count: an optional integer specifying the number of replacements to
                  make, values less than zero will replace all occurrences

      :returns: a copy of the string where `needle` replaces `replacement` up
                to `count` times
     */
    // TODO: not ideal - count and single allocation probably faster
    //                 - can special case on replacement|needle.length (0, 1)
    proc replace(needle: string, replacement: string, count: int = -1) : string {
      var result: string = this;
      var found: int = 0;
      var startIdx: int = 1;
      const localNeedle: string = needle.localize();
      const localReplacement: string = replacement.localize();

      while (count < 0) || (found < count) {
        const idx = result.find(localNeedle, startIdx..);
        if !idx then break;

        found += 1;
        result = result[..idx-1] + localReplacement + result[(idx + localNeedle.length)..];
        startIdx = idx + localReplacement.length;
      }
      return result;
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
    // TODO: specifying return type leads to un-inited string?
    iter split(sep: string, maxsplit: int = -1, ignoreEmpty: bool = false) /* : string */ {
      if !(maxsplit == 0 && ignoreEmpty && this.isEmptyString()) {
        const localThis: string = this.localize();
        const localSep: string = sep.localize();

        // really should be <, but we need to avoid returns and extra yields so
        // the iterator gets inlined
        var splitAll: bool = maxsplit <= 0;
        var splitCount: int = 0;

        var start: int = 1;
        var done: bool = false;
        while !done  {
          var chunk: string;
          var end: int;

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

          if !(ignoreEmpty && chunk.isEmptyString()) {
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

      :arg maxsplit: The number of times to split the string, negative values
                     indicate no limit.
     */
    // note: to improve performance, this code collapses several cases into a
    //       single yield statement, which makes it confusing to read
    // TODO: specifying return type leads to un-inited string?
    iter split(maxsplit: int = -1) /* : string */ {
      if !this.isEmptyString() {
        const localThis: string = this.localize();
        var done : bool = false;
        var yieldChunk : bool = false;
        var chunk : string;

        const noSplits : bool = maxsplit == 0;
        const limitSplits : bool = maxsplit > 0;
        var splitCount: int = 0;
        const iEnd = localThis.len - 1;

        var inChunk : bool = false;
        var chunkStart : int;

        for i in 0..iEnd {
          // emit whole string, unless all whitespace
          if noSplits {
            done = true;
            if !localThis.isSpace() then {
              chunk = localThis;
              yieldChunk = true;
            }
          } else {
            var b = localThis.buff[i];
            var bSpace = byte_isWhitespace(b);
            // first char of a chunk
            if !(inChunk || bSpace) {
              chunkStart = i + 1; // 0-based buff -> 1-based range
              inChunk = true;
              if i == iEnd {
                chunk = localThis[chunkStart..];
                yieldChunk = true;
                done = true;
              }
            } else if inChunk {
              // first char out of a chunk
              if bSpace {
                splitCount += 1;
                // last split under limit
                if limitSplits && splitCount > maxsplit {
                  chunk = localThis[chunkStart..];
                  yieldChunk = true;
                  done = true;
                // no limit
                } else {
                  chunk = localThis[chunkStart..i];
                  yieldChunk = true;
                  inChunk = false;
                }
              // out of chars
              } else if i == iEnd {
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

    proc join(const ref S: string ...) : string {
      return _join(S);
    }

    /*
      Same as the varargs version, but with a homogeneous tuple of strings.

      .. code-block:: chapel

          var x = "|".join("a","10","d");
          writeln(x); // prints: "a|10|d"
     */
    proc join(const ref S) : string where isTuple(S) {
      if !isHomogeneousTuple(S) || !isString(S[1]) then
        compilerError("join() on tuples only handles homogeneous tuples of strings");
      return _join(S);
    }

    /*
      Same as the varargs version, but with all the strings in an array.

      .. code-block:: chapel

          var x = "|".join(["a","10","d"]);
          writeln(x); // prints: "a|10|d"
     */
    proc join(const ref S: [] string) : string {
      return _join(S);
    }

    proc _join(const ref S) : string where isTuple(S) || isArray(S) {
      if S.size == 0 {
        return '';
      } else if S.size == 1 {
        // TODO: ensures copy, clean up when no longer needed
        var ret: string;
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

        var joined: string;
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
        return joined;
      }
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
    proc strip(chars: string = " \t\r\n", leading=true, trailing=true) : string {
      if this.isEmptyString() then return "";
      if chars.isEmptyString() then return this;

      const localThis: string = this.localize();
      const localChars: string = chars.localize();

      var start = 1;
      var end = localThis.len;

      if leading {
        label outer for i in 0..#localThis.len {
          for j in 0..#localChars.len {
            if localThis.buff[i] == localChars.buff[j] {
              start += 1;
              continue outer;
            }
          }
          break;
        }
      }

      if trailing {
        label outer for i in 0..#localThis.len by -1 {
          for j in 0..#localChars.len {
            if localThis.buff[i] == localChars.buff[j] {
              end -= 1;
              continue outer;
            }
          }
          break;
        }
      }

      return localThis[start..end];
    }

    /*
      Splits the string on `sep` into a `3*string` consisting of the section
      before `sep`, `sep`, and the section after `sep`. If `sep` is not found,
      the tuple will contain the whole string, and then two empty strings.
    */
    // TODO: I could make this and other routines that use find faster by
    // making a version of search helper that only takes in local strings and
    // localizing in the calling function
    proc const partition(sep: string) : 3*string {
      const idx = this.find(sep);
      if idx != 0 {
        return (this[..idx-1], sep, this[idx+sep.length..]);
      } else {
        return (this, "", "");
      }
    }

    /*
     Checks if all the characters in the string are either uppercase (A-Z) or
     uncased (not a letter).

      :returns: * `true`  -- if the string contains at least one uppercase
                             character and no lowercase characters, ignoring
                             uncased characters.
                * `false` -- otherwise
     */
    proc isUpper() : bool {
      if this.isEmptyString() then return false;

      var result: bool;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        var locale_result = false;
        for i in 0..#this.len {
          const b = buff[i];
          if byte_isLower(b) {
            locale_result = false;
            break;
          } else if !locale_result && byte_isUpper(b) {
            locale_result = true;
          }
          result = locale_result;
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the string are either lowercase (a-z) or
     uncased (not a letter).

      :returns: * `true`  -- when there are no uppercase characters in the string.
                * `false` -- otherwise
     */
    proc isLower() : bool {
      if this.isEmptyString() then return false;
      var result: bool = false;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if byte_isUpper(b) {
            result = false;
            break;
          } else if !result && byte_isLower(b) {
            result = true;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the string are whitespace (' ', '\t',
     '\n', '\v', '\f', '\r').

      :returns: * `true`  -- when all the characters are whitespace.
                * `false` -- otherwise
     */
    proc isSpace() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !(byte_isWhitespace(b)) {
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
    proc isAlpha() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !byte_isAlpha(b) {
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
    proc isDigit() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !byte_isDigit(b) {
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
    proc isAlnum() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !(byte_isAlpha(b) || byte_isDigit(b)) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the string are printable. Characters are
     defined as being printable if they are within the range of `0x20 - 0x7e`
     including the bounds.

      :returns: * `true`  -- when the characters are printable.
                * `false` -- otherwise
     */
    proc isPrintable() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const char = buff[i];
          if char <= 0x1f  || char == 0x7f {
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
    proc isTitle() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        param UN = 0, UPPER = 1, LOWER = 2;
        var last = UN;
        for i in 0..#this.len {
          const b = buff[i];
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
      :returns: A new string with all uppercase characters replaced with their
                lowercase counterpart.
    */
    proc toLower() : string {
      var result: string = this;
      if result.isEmptyString() then return result;

      for i in 0..#result.len {
        const b = result.buff[i];
        if byte_isUpper(b) {
          // We can just add or subtract 0x20 to change between upper and lower
          result.buff[i] = b + 0x20;
        }
      }
      return result;
    }

    /*
      :returns: A new string with all lowercase characters replaced with their
                uppercase counterpart.
    */
    proc toUpper() : string {
      var result: string = this;
      if result.isEmptyString() then return result;

      for i in 0..#result.len {
        const b = result.buff[i];
        if byte_isLower(b) {
          result.buff[i] = b - 0x20;
        }
      }
      return result;
    }

    /*
      :returns: A new string with all cased characters following an uncased
                character converted to uppercase, and all cased characters
                following another cased character converted to lowercase.
     */
    proc toTitle() : string {
      var result: string = this;
      if result.isEmptyString() then return result;

      param UN = 0, LETTER = 1;
      var last = UN;
      for i in 0..#result.len {
        const b = result.buff[i];
        if byte_isAlpha(b) {
          if last == UN {
            last = LETTER;
            if byte_isLower(b) {
              result.buff[i] = b - 0x20;
            }
          } else { // last == LETTER
            if byte_isUpper(b) {
              result.buff[i] = b + 0x20;
            }
          }
        } else {
          // Uncased elements
          last = UN;
        }
      }
      return result;
    }

    /*
      :returns: A new string with the first character in uppercase (if it is a
                case character), and all other case characters in lowercase.
                Uncased characters are copied with no changes.
    */
    pragma "no doc"
    proc capitalize() : string {
      var result: string = this.toLower();
      if result.isEmptyString() then return result;

      var b = result.buff[0];
      if byte_isLower(b) {
        result.buff[0] = b - 0x20;
      }
      return result;
    }

  } // end record string


  // We'd like this to be by ref, but doing so leads to an internal
  // compiler error.  See
  // $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
  pragma "donor fn"
  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(s: string) {
    // This pragma may be unnecessary.
    pragma "no auto destroy"
    var ret: string;
    const slen = s.len; // cache the remote copy of len
    if slen != 0 {
      if _local || s.locale_id == chpl_nodeID {
        if s.owned {
          ret.buff = chpl_here_alloc(s._size,
                                    offset_STR_COPY_DATA): bufferType;
          c_memcpy(ret.buff, s.buff, s.len);
          ret.buff[s.len] = 0;
        } else {
          ret.buff = s.buff;
        }
        ret.owned = s.owned;
      } else {
        ret.buff = copyRemoteBuffer(s.locale_id, s.buff, slen);
        ret.owned = true;
      }
      ret.len = slen;
      ret._size = slen+1;
    }
    return ret;
  }

  /*
   * NOTES: Just a word on memory allocation in the generated code.
   * Any function that returns a string copies the returned value
   * including the c_string via a call to chpl__initCopy().  The
   * copied value is returned, and the original is destroyed on the
   * way out.  I'd really like to figure out a way to pass back the
   * original string without copying it.  I think I might be able to
   * do it by putting some sort of flag in the string record that is
   * used by initCopy().
   * TODO: Check if ^ is still true w/ the new AMM
   * TODO: Do we need an initCopy for strings?  If not, this clause can be removed.
   */
  pragma "init copy fn"
  pragma "no doc"
  proc chpl__initCopy(s: string) {
    // This pragma may be unnecessary.
    pragma "no auto destroy"
    var ret: string;
    const slen = s.len; // cache the remote copy of len
    if slen != 0 {
      if _local || s.locale_id == chpl_nodeID {
        if s.owned {
          ret.buff = chpl_here_alloc(s._size,
                                    offset_STR_COPY_DATA): bufferType;
          c_memcpy(ret.buff, s.buff, s.len);
          ret.buff[s.len] = 0;
        } else {
          ret.buff = s.buff;
        }
        ret.owned = s.owned;
      } else {
        ret.buff = copyRemoteBuffer(s.locale_id, s.buff, slen);
        ret.owned = true;
      }
      ret.len = slen;
      ret._size = slen+1;
    }
    return ret;
  }

  //
  // Assignment functions
  //
  /*
     Copies the string `rhs` into the string `lhs`.
  */
  proc =(ref lhs: string, rhs: string) {
    inline proc helpMe(ref lhs: string, rhs: string) {
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
     Copies the c_string `rhs_c` into the string `lhs`.

     Halts if `lhs` is a remote string.
  */
  proc =(ref lhs: string, rhs_c: c_string) {
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
     :returns: A new string which is the result of concatenating `s0` and `s1`
  */
  proc +(s0: string, s1: string) {
    // cache lengths locally
    const s0len = s0.len;
    if s0len == 0 then return s1;
    const s1len = s1.len;
    if s1len == 0 then return s0;

    var ret: string;
    ret.len = s0len + s1len;
    const allocSize = chpl_here_good_alloc_size(ret.len+1);
    ret._size = allocSize;
    ret.buff = chpl_here_alloc(allocSize,
                              offset_STR_COPY_DATA): bufferType;
    ret.owned = true;

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
     :returns: A new string which is the result of repeating `s` `n` times.
               If `n` is less than or equal to 0, an empty string is returned.

     For example:

     .. code-block:: chapel

        writeln("Hello! " * 3);

     Results in::

       Hello! Hello! Hello!
  */
  proc *(s: string, n: integral) {
    if n <= 0 then return "";

    const sLen = s.length;
    if sLen == 0 then return "";

    var ret: string;
    ret.len = sLen * n; // TODO: check for overflow
    const allocSize = chpl_here_good_alloc_size(ret.len+1);
    ret._size = allocSize;
    ret.buff = chpl_here_alloc(allocSize,
                              offset_STR_COPY_DATA): bufferType;
    ret.owned = true;

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

  // Concatenation with other types is done by casting to string
  private inline proc concatHelp(s: string, x:?t) where t != string {
    var cs = x:string;
    const ret = s + cs;
    return ret;
  }

  private inline proc concatHelp(x:?t, s: string) where t != string  {
    var cs = x:string;
    const ret = cs + s;
    return ret;
  }

  /*
     The following concatenation functions return a new string which is the
     result of casting the non-string argument to a string, and concatenating
     that result with `s`.
  */
  inline proc +(s: string, x: numeric) return concatHelp(s, x);
  inline proc +(x: numeric, s: string) return concatHelp(x, s);
  inline proc +(s: string, x: enumerated) return concatHelp(s, x);
  inline proc +(x: enumerated, s: string) return concatHelp(x, s);
  inline proc +(s: string, x: bool) return concatHelp(s, x);
  inline proc +(x: bool, s: string) return concatHelp(x, s);

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
  inline proc +(param s: string, param x: integral) param
    return __primitive("string_concat", s, x:string);

  pragma "no doc"
  inline proc +(param x: integral, param s: string) param
    return __primitive("string_concat", x:string, s);

  pragma "no doc"
  inline proc +(param s: string, param x: enumerated) param
    return __primitive("string_concat", s, x:string);

  pragma "no doc"
  inline proc +(param x: enumerated, param s: string) param
    return __primitive("string_concat", x:string, s);

  pragma "no doc"
  inline proc +(param s: string, param x: bool) param
    return __primitive("string_concat", s, x:string);

  pragma "no doc"
  inline proc +(param x: bool, param s: string) param
    return __primitive("string_concat", x:string, s);

  pragma "no doc"
  inline proc ascii(param a: string) param return __primitive("ascii", a);

  pragma "no doc"
  inline proc param string.length param
    return __primitive("string_length", this);

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
    // if rhs is empty, nothing to do
    if rhs.len == 0 then return;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
      const rhsLen = rhs.len;
      const newLength = lhs.len+rhsLen; //TODO: check for overflow
      if lhs._size <= newLength {
        const newSize = chpl_here_good_alloc_size(
            max(newLength+1, lhs.len*chpl_stringGrowthFactor):int);

        if lhs.owned {
          lhs.buff = chpl_here_realloc(lhs.buff, newSize,
                                      offset_STR_COPY_DATA):bufferType;
        } else {
          var newBuff = chpl_here_alloc(newSize,
                                       offset_STR_COPY_DATA):bufferType;
          c_memcpy(newBuff, lhs.buff, lhs.len);
          lhs.buff = newBuff;
          lhs.owned = true;
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

  private inline proc _strcmp(a: string, b:string) : int {
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

  pragma "no doc"
  proc ==(a: string, b: string) : bool {
    inline proc doEq(a: string, b:string) {
      // TODO: is it better to have these outside of the do* fns?
      //       probably would be 2 extra gets worst case, but avoids doing a
      //       local copy if b is remote
      if a.len != b.len then return false;
      return _strcmp(a, b) == 0;
    }

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

    var localA: string = a.localize();
    var localB: string = b.localize();

    return doEq(localA, localB);
  }

  pragma "no doc"
  inline proc !=(a: string, b: string) : bool {
    return !(a == b);
  }

  pragma "no doc"
  inline proc <(a: string, b: string) : bool {
    inline proc doLt(a: string, b:string) {
      return _strcmp(a, b) < 0;
    }

    var localA: string = a.localize();
    var localB: string = b.localize();

    return doLt(localA, localB);
  }

  pragma "no doc"
  inline proc >(a: string, b: string) : bool {
    inline proc doGt(a: string, b:string) {
      return _strcmp(a, b) > 0;
    }

    var localA: string = a.localize();
    var localB: string = b.localize();

    return doGt(localA, localB);
  }

  pragma "no doc"
  inline proc <=(a: string, b: string) : bool {
    return !(a > b);
  }
  pragma "no doc"
  inline proc >=(a: string, b: string) : bool {
    return !(a < b);
  }


  //
  // Helper routines
  //
  private const uint_A = ascii('A');
  private const uint_Z = ascii('Z');
  private const uint_a = ascii('a');
  private const uint_z = ascii('z');
  private const uint_0 = ascii('0');
  private const uint_9 = ascii('9');
  private const uint_space    = ascii(' ');
  private const uint_tab      = ascii('\t');
  private const uint_newline  = ascii('\n');
  private const uint_return   = ascii('\r');

  private inline proc byte_isUpper(b: uint(8)) : bool {
    return b >= uint_A && b <= uint_Z;
  }

  private inline proc byte_isLower(b: uint(8)) : bool {
    return b >= uint_a && b <= uint_z;
  }

  private inline proc byte_isAlpha(b: uint(8)) : bool {
    return byte_isLower(b) || byte_isUpper(b);
  }

  private inline proc byte_isDigit(b: uint(8)) : bool {
    return b >= uint_0  && b <= uint_9;
  }

  private inline proc byte_isWhitespace(b: uint(8)) : bool {
    return b == uint_space
        || b == uint_tab
        || (b >= uint_newline && b <= uint_return);
  }

  //
  // ascii
  // TODO: replace with ordinal()
  //
  /*
     :returns: The byte value of the first character in `a` as an integer.
  */
  inline proc ascii(a: string) : uint(8) {
    if a.isEmptyString() then return 0;

    if _local || a.locale_id == chpl_nodeID {
      // the string must be local so we can index into buff
      return a.buff[0];
    } else {
      // a[1] grabs the first character as a string (making it local)
      return a[1].buff[0];
    }
  }

  /*
     :returns: A string with the single character with the ASCII value `i`.
  */
  inline proc asciiToString(i: uint(8)) {
    var buffer = chpl_here_alloc(2, offset_STR_COPY_DATA): bufferType;
    buffer[0] = i;
    buffer[1] = 0;
    var s = new string(buffer, 1, 2, owned=true, needToCopy=false);
    return s;
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
    var ret: string;
    ret.len = cs.length;
    ret._size = ret.len+1;
    ret.buff = if ret.len > 0
      then __primitive("string_copy", cs): bufferType
      else nil;
    ret.owned = true;

    return ret;
  }

  //
  // hashing support
  //

  pragma "no doc"
  inline proc chpl__defaultHash(x : string): uint {
    // Use djb2 (Dan Bernstein in comp.lang.c), XOR version
    var hash: int(64) = 5381;
    for c in 0..#(x.length) {
      hash = ((hash << 5) + hash) ^ x.buff[c];
    }
    return hash;
  }

  //
  // Developer Extras
  //
  pragma "no doc"
  proc chpldev_refToString(ref arg) : string {
    // print out the address of class references as well
    proc chpldev_classToString(x: object) : string
      return " (class = " + __primitive("ref to string", x):string + ")";
    proc chpldev_classToString(x) : string return "";

    return __primitive("ref to string", arg):string + chpldev_classToString(arg);
  }
}
