/*
 * Copyright 2004-2015 Cray Inc.
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
 * - An empty string is represted by len == 0 and buff == nil.
 *
 * - It is assumed the bufferType is a local-only type, so we never
 *   make a remote copy of one passed in by the user, though remote
 *   copies are made of internal bufferType variables.
 *
 */

module BaseStringType {
  // TODO: figure out why I cant move this defintion into `module String`
  type bufferType = c_ptr(uint(8));
}

// Chapel Strings
module String {
  use BaseStringType;
  use CString;
  use SysCTypes;
  use StringCasts;

  //
  // Externs and constants used to implement strings
  //
  // TODO: mark most of these as private or move elsewhere

  param chpl_string_min_alloc_size: int = 16;
  // Growth factor to use when extending the buffer for appends
  config param chpl_stringGrowthFactor = 1.5;
  extern proc chpl_mem_goodAllocSize(minSize: size_t) : size_t;

  extern type chpl_mem_descInt_t;

  // We use this as a shortcut to get at here.id without actually constructing
  // a locale object. Used when determining if we should make a remote transfer.
  // This extern is also defined in ChapelLocale.
  //extern var chpl_nodeID: chpl_nodeID_t;

  // TODO: hook into chpl_here_alloc and friends somehow
  // The runtime exits on errors for these, no need to check for nil on return
  pragma "insert line file info"
  extern proc chpl_mem_alloc(size: size_t,
                             description: chpl_mem_descInt_t): c_void_ptr;

  pragma "insert line file info"
  extern proc chpl_mem_calloc(number: size_t, size: size_t,
                              description: chpl_mem_descInt_t) : c_void_ptr;

  pragma "insert line file info"
  extern proc chpl_mem_realloc(ptr: c_ptr, size: size_t,
                               description: chpl_mem_descInt_t) : c_void_ptr;

  pragma "insert line file info"
  extern proc chpl_mem_free(ptr: c_ptr): void;

  // TODO: define my own mem descriptors?
  extern const CHPL_RT_MD_STR_COPY_REMOTE: chpl_mem_descInt_t;
  extern const CHPL_RT_MD_STR_COPY_DATA: chpl_mem_descInt_t;

  private inline proc chpl_string_comm_get(dest: bufferType, src_loc_id: int(64),
                                   src_addr: bufferType, len: size_t) {
    __primitive("chpl_comm_get", dest, src_loc_id, src_addr, len);
  }

  proc copyRemoteBuffer(src_loc_id: int(64), src_addr: bufferType, len: int): bufferType {
      const dest = chpl_mem_alloc((len+1).safeCast(size_t), CHPL_RT_MD_STR_COPY_REMOTE): bufferType;
      chpl_string_comm_get(dest, src_loc_id, src_addr, len.safeCast(size_t));
      dest[len] = 0;
      return dest;
  }

  extern proc memmove(destination: c_ptr, const source: c_ptr, num: size_t);
  extern proc memcpy(destination: c_ptr, const source: c_ptr, num: size_t);
  extern proc memcmp(s1: c_ptr, s2: c_ptr, n: size_t) : c_int;

  config param debugStrings = false;

  //
  // String Implementation
  //
  // TODO: We should be able to remove "ignore noinit", but doing so causes
  // memory leaks in various places. Investigate why later and add noinit back
  // in when possible.
  pragma "ignore noinit"
  pragma "no default functions" // avoid the default (read|write)This routines
  record string {
    var len: int = 0; // length of string in bytes
    var _size: int = 0; // size of the buffer we own
    var buff: bufferType = nil;
    var owned: bool = true;
    var locale_id /* : chpl_nodeID_t */ = chpl_nodeID;

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
            const allocSize = chpl_mem_goodAllocSize((sLen+1).safeCast(size_t));
            this.buff = chpl_mem_alloc(allocSize,
                                       CHPL_RT_MD_STR_COPY_DATA): bufferType;
            memcpy(this.buff, s.buff, s.len.safeCast(size_t));
            this.buff[sLen] = 0;
            this._size = allocSize.safeCast(int);
          } else {
            this.buff = s.buff;
            this._size = s._size;
          }
        }
      }
    }

    // This constructor can cause a leak if owned = false and needToCopy = true
    proc string(buff: bufferType, length: int, size: int,
                owned: bool = true, needToCopy: bool = true) {
      this.owned = owned;
      this.reinitString(buff, length, size, needToCopy);
    }

    proc ref ~string() {
      if owned && !this.isEmptyString() {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
          chpl_mem_free(this.buff);
        }
      }
    }

    // This is assumed to be called from this.locale
    proc ref reinitString(buf: bufferType, s_len: int, size: int,
                          needToCopy:bool = true) {
      if this.isEmptyString() {
        if (s_len == 0) || (buf == nil) then return; // nothing to do
      }

      // If the this.buff is longer than buf, then reuse the buffer if we are
      // allowed to (this.owned == true)
      if s_len != 0 {
        if needToCopy {
          if !this.owned || s_len+1 > this._size {
            // If the new string is too big for our current buffer or we dont
            // own our current buffer then we need a new one.
            if this.owned && !this.isEmptyString() then
              chpl_mem_free(this.buff);
            // TODO: should I just allocate 'size' bytes?
            const allocSize = chpl_mem_goodAllocSize((s_len+1).safeCast(size_t));
            this.buff = chpl_mem_alloc(allocSize,
                                       CHPL_RT_MD_STR_COPY_DATA):bufferType;
            this.buff[s_len] = 0;
            this._size = allocSize.safeCast(int);
            // We just allocated a buffer, make sure to free it later
            this.owned = true;
          }
          memmove(this.buff, buf, s_len.safeCast(size_t));
        } else {
          if this.owned && !this.isEmptyString() then
            chpl_mem_free(this.buff);
          this.buff = buf;
          this._size = size;
        }
      } else {
        // free the old buffer
        if this.owned && !this.isEmptyString() then chpl_mem_free(this.buff);
        this.buff = nil;
        this._size = 0;
      }

      this.len = s_len;
    }

    inline proc c_str(): c_string {
      inline proc _cast(type t, x) where t:c_string && x.type:bufferType {
        return __primitive("cast", t, x);
      }

      if this.locale_id != chpl_nodeID then
        halt("Cannot call .c_str() on a remote string");

      return this.buff:c_string;
    }

    inline proc param c_str() param : c_string {
      inline proc _cast(type t, x) where t:c_string && x.type:string {
        return __primitive("cast", t, x);
      }
      return this:c_string; // folded out in resolution
    }

    inline proc length return len;
    //inline proc size return len;

    // Localize gets a version of this that is on the currently executing
    // locale. If this is already on the current locale we return a shallow
    // copy, otherwise a remote copy is performed.
    inline proc localize() : string {
        if _local || this.locale_id == chpl_nodeID {
          return new string(this, owned=false);
        } else {
          const x:string = this; // assignment makes it local
          return x;
        }
    }

    iter these() : string {
      for i in 1..this.len {
        yield this[i];
      }
    }

    // TODO: add comments
    // 1s based indexing into the string
    proc this(i: int) : string {
      if i <= 0 || i > this.len then halt("index out of bounds of string");

      var ret: string;
      const newSize = chpl_mem_goodAllocSize(2);
      ret._size = max(chpl_string_min_alloc_size, newSize.safeCast(int));
      ret.len = 1;
      ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                                CHPL_RT_MD_STR_COPY_DATA): bufferType;
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
    proc _getView(r:range(?)) {
      //TODO: don't use halt here, or at least wrap in a bounds checks param
      //TODO: halt()s should use string.writef at some point.
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
      const ret = r[1:r.idxType..#(this.len:r.idxType)];
      return ret;
    }

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
        const newSize = chpl_mem_goodAllocSize((ret.len+1).safeCast(size_t));
        ret._size = max(chpl_string_min_alloc_size, newSize.safeCast(int));
        // FIXME: I was dumb here, just copy the correct region over in
        // multi-locale and use that as the string buffer. No need to copy stuff
        // about after pulling it across.
        ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                                  CHPL_RT_MD_STR_COPY_DATA): bufferType;

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

        for (r2_i, i) in zip(r2, 0..) {
          ret.buff[i] = thisBuff[r2_i-1];
        }
        ret.buff[ret.len] = 0;

        if remoteThis then chpl_mem_free(thisBuff);
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

    inline proc isEmptyString() : bool {
      return this.len == 0; // this should be enough of a check
    }

    // These should never be called (but are default functions for records)
    pragma "no doc"
    proc writeThis(f: Writer) {
      compilerError("not implemented: writeThis");
    }
    pragma "no doc"
    proc readThis(f: Reader) {
      compilerError("not implemented: readThis");
    }

    // TODO: could use a multi-pattern search or some variant when there are
    // multiple needles. Probably wouldn't be worth the overhead for small
    // needles though
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
            const result = memcmp(this.buff, localNeedle.buff,
                                  localNeedle.len.safeCast(size_t));
            ret = result == 0;
          } else {
            var offset = this.len-localNeedle.len;
            const result = memcmp(this.buff+offset, localNeedle.buff,
                                  localNeedle.len.safeCast(size_t));
            ret = result == 0;
          }
          if ret == true then break;
        }
      }
      return ret;
    }

    proc startsWith(needles: string ...) : bool {
      return _startsEndsWith((...needles), fromLeft=true);
    }

    proc endsWith(needles: string ...) : bool {
      return _startsEndsWith((...needles), fromLeft=false);
    }


    // Helper function that uses a param bool to toggle between count and find
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    //
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

    // Returns the index of the first occurrence of a substring within a
    // string or 0 if the substring is not in the string.
    // TODO: better name than region?
    proc find(needle: string, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=false);
    }

    proc count(needle: string, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=true);
    }

    proc rfind(needle: string, region: range(?) = 1..) : int {
      return _search_helper(needle, region, count=false, fromLeft=false);
    }

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

    // TODO: Make this support spliting on whitespace rather than just a space
    // TODO: specifying return type leads to un-inited string?
    iter split(maxsplit: int = -1, ignoreEmpty: bool = false) /* : string*/ {
      for s in this.split(" ", maxsplit, ignoreEmpty) {
        yield s;
      }
    }

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

    // TODO: could rewrite to have cleaner logic / more efficient for edge cases
    proc join(S: [] string) : string {
      var newSize: int = 0;
      var ret: string;
      if S.size > 1 {
        for s in S {
          newSize += s.length;
        }
        newSize += this.len*(S.size-1);
        ret.len = newSize;
        const allocSize = chpl_mem_goodAllocSize((ret.len+1).safeCast(size_t));
        ret._size = allocSize.safeCast(int);
        ret.buff = chpl_mem_alloc(allocSize,
                                  CHPL_RT_MD_STR_COPY_DATA): bufferType;
        var offset = 0;
        var first = true;
        for s in S {
          if !first && this.len != 0 {
            memcpy(ret.buff+offset, this.buff, this.len.safeCast(size_t));
            offset += this.len;
          }
          var sLen = s.len;
          if sLen != 0 {
            if _local || s.locale_id == chpl_nodeID {
              memcpy(ret.buff+offset, s.buff, sLen.safeCast(size_t));
            } else {
              chpl_string_comm_get(ret.buff+offset, s.locale_id,
                                  s.buff, sLen.safeCast(size_t));
            }
            offset += sLen;
          }
          first = false;
        }
      } else if S.size == 1 {
        ret = S[0];
      }
      return ret;
    }

    proc strip(chars: string, leading=true, trailing=true) : string {
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

    inline proc strip(leading=true, trailing=true) : string {
      return this.strip(" \t\r\n", leading, trailing);
    }

    // TODO: I could make this and other routines that use find faster by
    // making a version of search helper that only takes in local strings and
    // localizing in the calling function
    proc partition(sep: string) : 3*string {
      const idx = this.find(sep);
      if idx != 0 {
        return (this[..idx-1], sep, this[idx+sep.length..]);
      } else {
        return (this, "", "");
      }
    }

    proc isUpper() : bool {
      var result: bool = false;
      if this.isEmptyString() then return false;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if _byte_isLower(b) {
            result = false;
            break;
          } else if !result && _byte_isUpper(b) {
            result = true;
          }
        }
      }
      return result;
    }

    proc isLower() : bool {
      if this.isEmptyString() then return false;
      var result: bool = false;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if _byte_isUpper(b) {
            result = false;
            break;
          } else if !result && _byte_isLower(b) {
            result = true;
          }
        }
      }
      return result;
    }

    proc isSpace() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !((b == ascii(' ')) ||
               (b == ascii('\t')) ||
               (b >= ascii('\n') && b <= ascii('\r'))) { // \n \v \f \r
            result = false;
            break;
          }
        }
      }
      return result;
    }

    proc isAlpha() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !_byte_isAlpha(b) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    proc isDigit() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !_byte_isDigit(b) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    proc isAlnum() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const b = buff[i];
          if !(_byte_isAlpha(b) || _byte_isDigit(b)) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    proc isPrintable() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for i in 0..#this.len {
          const char = buff[i];
          if char < 0x1f  || char == 0x7f {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    proc isTitle() : bool {
      if this.isEmptyString() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        param UN = 0, UPPER = 1, LOWER = 2;
        var last = UN;
        for i in 0..#this.len {
          const b = buff[i];
          if _byte_isLower(b) {
            if last == UPPER || last == LOWER {
              last = LOWER;
            } else { // last == UN
              result = false;
              break;
            }
          }
          else if _byte_isUpper(b) {
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

    proc toLower() : string {
      var result: string = this;
      if result.isEmptyString() then return result;

      for i in 0..#result.len {
        const b = result.buff[i];
        if _byte_isUpper(b) {
          // We can just add or subtract 0x20 to change between upper and lower
          result.buff[i] = b + 0x20;
        }
      }
      return result;
    }

    proc toUpper() : string {
      var result: string = this;
      if result.isEmptyString() then return result;

      for i in 0..#result.len {
        const b = result.buff[i];
        if _byte_isLower(b) {
          result.buff[i] = b - 0x20;
        }
      }
      return result;
    }

    proc toTitle() : string {
      var result: string = this;
      if result.isEmptyString() then return result;

      param UN = 0, LETTER = 1;
      var last = UN;
      for i in 0..#result.len {
        const b = result.buff[i];
        if _byte_isAlpha(b) {
          if last == UN {
            last = LETTER;
            if _byte_isLower(b) {
              result.buff[i] = b - 0x20;
            }
          } else { // last == LETTER
            if _byte_isUpper(b) {
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

    proc capitalize() : string {
      var result: string = this.toLower();
      if result.isEmptyString() then return result;

      var b = result.buff[0];
      if _byte_isLower(b) { // Only change alpha
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
  proc chpl__autoCopy(s: string) {
    // This pragma may be unnecessary.
    pragma "no auto destroy"
    var ret: string;
    const slen = s.len; // cache the remote copy of len
    if slen != 0 {
      if _local || s.locale_id == chpl_nodeID {
        if s.owned {
          ret.buff = chpl_mem_alloc(s._size.safeCast(size_t),
                                    CHPL_RT_MD_STR_COPY_DATA): bufferType;
          memcpy(ret.buff, s.buff, s.len.safeCast(size_t));
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
  proc chpl__initCopy(s: string) {
    // This pragma may be unnecessary.
    pragma "no auto destroy"
    var ret: string;
    const slen = s.len; // cache the remote copy of len
    if slen != 0 {
      if _local || s.locale_id == chpl_nodeID {
        if s.owned {
          ret.buff = chpl_mem_alloc(s._size.safeCast(size_t),
                                    CHPL_RT_MD_STR_COPY_DATA): bufferType;
          memcpy(ret.buff, s.buff, s.len.safeCast(size_t));
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
  proc +(s0: string, s1: string) {
    // cache lengths locally
    const s0len = s0.len;
    if s0len == 0 then return s1;
    const s1len = s1.len;
    if s1len == 0 then return s0;

    var ret: string;
    ret.len = s0len + s1len;
    const allocSize = chpl_mem_goodAllocSize((ret.len+1).safeCast(size_t));
    ret._size = allocSize.safeCast(int);
    ret.buff = chpl_mem_alloc(allocSize,
                              CHPL_RT_MD_STR_COPY_DATA): bufferType;
    ret.owned = true;

    const s0remote = s0.locale_id != chpl_nodeID;
    if s0remote {
      chpl_string_comm_get(ret.buff, s0.locale_id,
                           s0.buff, s0len.safeCast(size_t));
    } else {
      memcpy(ret.buff, s0.buff, s0len.safeCast(size_t));
    }

    const s1remote = s1.locale_id != chpl_nodeID;
    if s1remote {
      chpl_string_comm_get(ret.buff+s0len, s1.locale_id,
                           s1.buff, s1len.safeCast(size_t));
    } else {
      memcpy(ret.buff+s0len, s1.buff, s1len.safeCast(size_t));
    }
    ret.buff[ret.len] = 0;

    return ret;
  }

  proc *(s: string, n: integral) {
    if n <= 0 then return "";

    const sLen = s.length;
    if sLen == 0 then return "";

    var ret: string;
    ret.len = sLen * n; // TODO: check for overflow
    const allocSize = chpl_mem_goodAllocSize((ret.len+1).safeCast(size_t));
    ret._size = allocSize.safeCast(int);
    ret.buff = chpl_mem_alloc(allocSize,
                              CHPL_RT_MD_STR_COPY_DATA): bufferType;
    ret.owned = true;

    const sRemote = s.locale_id != chpl_nodeID;
    if sRemote {
      chpl_string_comm_get(ret.buff, s.locale_id,
                           s.buff, sLen.safeCast(size_t));
    } else {
      memcpy(ret.buff, s.buff, sLen.safeCast(size_t));
    }

    var iterations = n-1;
    var offset = sLen;
    for i in 1..iterations {
      memcpy(ret.buff+offset, ret.buff, sLen.safeCast(size_t));
      offset += sLen;
    }
    ret.buff[ret.len] = 0;

    return ret;
  }

  // Concatenation with other types is done by casting to string
  inline proc concatHelp(s: string, x:?t) where t != string {
    var cs = x:string;
    const ret = s + cs;
    return ret;
  }

  inline proc concatHelp(x:?t, s: string) where t != string  {
    var cs = x:string;
    const ret = cs + s;
    return ret;
  }

  inline proc +(s: string, x: numeric) return concatHelp(s, x);
  inline proc +(x: numeric, s: string) return concatHelp(x, s);
  inline proc +(s: string, x: enumerated) return concatHelp(s, x);
  inline proc +(x: enumerated, s: string) return concatHelp(x, s);
  inline proc +(s: string, x: bool) return concatHelp(s, x);
  inline proc +(x: bool, s: string) return concatHelp(x, s);

  //
  // Param procs
  //
  proc typeToString(type t) param {
    return __primitive("typeToString", t);
  }

  proc typeToString(x) param {
    compilerError("typeToString()'s argument must be a type, not a value");
  }

  inline proc ==(param s0: string, param s1: string) param  {
    return __primitive("string_compare", s0, s1) == 0;
  }

  inline proc !=(param s0: string, param s1: string) param {
    return __primitive("string_compare", s0, s1) != 0;
  }

  inline proc <=(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) <= 0);
  }

  inline proc >=(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) >= 0);
  }

  inline proc <(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) < 0);
  }

  inline proc >(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) > 0);
  }

  inline proc +(param a: string, param b: string) param
    return __primitive("string_concat", a, b);

  inline proc +(param s: string, param x: integral) param
    return __primitive("string_concat", s, x:string);

  inline proc +(param x: integral, param s: string) param
    return __primitive("string_concat", x:string, s);

  inline proc +(param s: string, param x: enumerated) param
    return __primitive("string_concat", s, x:string);

  inline proc +(param x: enumerated, param s: string) param
    return __primitive("string_concat", x:string, s);

  inline proc +(param s: string, param x: bool) param
    return __primitive("string_concat", s, x:string);

  inline proc +(param x: bool, param s: string) param
    return __primitive("string_concat", x:string, s);

  inline proc ascii(param a: string) param return __primitive("ascii", a);

  inline proc param string.length param
    return __primitive("string_length", this);

  inline proc _string_contains(param a: string, param b: string) param
    return __primitive("string_contains", a, b);


  //
  // Append
  //
  proc +=(ref lhs: string, rhs: string) : void {
    // if rhs is empty, nothing to do
    if rhs.len == 0 then return;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
      const rhsLen = rhs.len;
      const newLength = lhs.len+rhsLen; //TODO: check for overflow
      if lhs._size <= newLength {
        const newSize = chpl_mem_goodAllocSize(
            max(newLength+1,
                (lhs.len * chpl_stringGrowthFactor):int).safeCast(size_t));

        if lhs.owned {
          lhs.buff = chpl_mem_realloc(lhs.buff, newSize.safeCast(size_t),
                                      CHPL_RT_MD_STR_COPY_DATA):bufferType;
        } else {
          var newBuff = chpl_mem_alloc(newSize.safeCast(size_t),
                                       CHPL_RT_MD_STR_COPY_DATA):bufferType;
          memcpy(newBuff, lhs.buff, lhs.len.safeCast(size_t));
          lhs.buff = newBuff;
          lhs.owned = true;
        }

        lhs._size = newSize.safeCast(int);
      }
      const rhsRemote = rhs.locale_id != chpl_nodeID;
      if rhsRemote {
        chpl_string_comm_get(lhs.buff+lhs.len, rhs.locale_id,
                              rhs.buff, rhsLen.safeCast(size_t));
      } else {
        memcpy(lhs.buff+lhs.len, rhs.buff, rhsLen.safeCast(size_t));
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

  inline proc _strcmp(a: string, b:string) : int {
    // Assumes a and b are on same locale and not empty.
    const size = min(a.len, b.len);
    const result =  memcmp(a.buff, b.buff, size.safeCast(size_t));

    if (result == 0) {
      // Handle cases where one string is the beginning of the other
      if (size < a.len) then return 1;
      if (size < b.len) then return -1;
    }
    return result;
  }

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

  inline proc !=(a: string, b: string) : bool {
    return !(a == b);
  }

  inline proc <(a: string, b: string) : bool {
    inline proc doLt(a: string, b:string) {
      return _strcmp(a, b) < 0;
    }

    var localA: string = a.localize();
    var localB: string = b.localize();

    return doLt(localA, localB);
  }

  inline proc >(a: string, b: string) : bool {
    inline proc doGt(a: string, b:string) {
      return _strcmp(a, b) > 0;
    }

    var localA: string = a.localize();
    var localB: string = b.localize();

    return doGt(localA, localB);
  }

  inline proc <=(a: string, b: string) : bool {
    return !(a > b);
  }
  inline proc >=(a: string, b: string) : bool {
    return !(a < b);
  }


  //
  // Helper routines
  //
  private inline proc _byte_isUpper(b: uint(8)) : bool {
    return b >= ascii('A') && b <= ascii('Z');
  }

  private inline proc _byte_isLower(b: uint(8)) : bool {
    return b >= ascii('a') && b <= ascii('z');
  }

  private inline proc _byte_isAlpha(b: uint(8)) : bool {
    return b >= ascii('A')  && b <= ascii('z');
  }

  private inline proc _byte_isDigit(b: uint(8)) : bool {
    return b >= ascii('0')  && b <= ascii('9');
  }


  //
  // ascii
  // TODO: replace with ordinal()
  //
  inline proc ascii(a: string) : int(32) {
    if a.isEmptyString() then return 0;

    if _local || a.locale_id == chpl_nodeID {
      // the string must be local so we can index into buff
      return a.buff[0]:int(32);
    } else {
      // a[1] grabs the first character as a string (making it local)
      return a[1].buff[0]:int(32);
    }
  }


  //
  // Casts (casts to & from other primitive types are in StringCasts)
  //

  // TODO: remove this and fix the folding out of string casts
  // Yes this is invoked sometimes. In the long run, however,
  // we'd like the compiler to eliminate casts to the same type instead.
  inline proc _cast(type t, x: c_string) where t == c_string {
    return x;
  }

  inline proc _cast(type t, cs: c_string) where t == bufferType {
    return __primitive("cast", t, cs);
  }

  inline proc _cast(type t, cs: c_string_copy) where t == bufferType {
    return __primitive("cast", t, cs);
  }

  // Cast from c_string to string
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
  // Developer Extras
  //
  pragma "no doc"
  proc chpldev_refToString(ref arg) : string {
    // print out the address of class references as well
    proc chpldev_classToString(x: object) : string
      return " (class = " + __primitive("ref to string", x) + ")";
    proc chpldev_classToString(x) : string return "";

    return __primitive("ref to string", arg) + chpldev_classToString(arg);
  }
}
