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

//TODO: I dont think this module is overly useful anymore
module BaseStringType {
  use CString;
  use SysCTypes;

  type bufferType = c_ptr(uint(8));
  //param bufferTypeString: c_string = "c_ptr(uint(8))";
  param chpl_string_min_alloc_size: int = 16;
  // Growth factor to use when extending the buffer for appends
  config param chpl_stringGrowthFactor = 1.5;
  extern proc chpl_mem_goodAllocSize(minSize: size_t) : size_t;

  extern type chpl_mem_descInt_t;

  // We use this as a shortcut to get at here.id without actually constructing
  // a locale object. Used when determining if we should make a remote transfer.
  // This extern is also defined in ChapelLocale, but if it isn't defined here
  // as well we will get a duplicate symbol error. I believe this is due to
  // module load ordering issues.
  // TODO: make a proc in ChapelLocale that returns this rather than using the extern
  extern var chpl_nodeID: chpl_nodeID_t;

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

  inline proc chpl_string_comm_get(dest: bufferType, src_loc_id: int(64),
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

  // pointer arithmetic used for strings
  inline proc +(a: c_ptr, b: integral) return __primitive("+", a, b);

  config param debugStrings = false;
}

// Chapel Strings
module String {
  use BaseStringType;
  use StringCasts;

  inline proc chpl_debug_string_print(s: c_string) {
    if debugStrings {
      extern proc printf(format: c_string, x...);
      printf("%s\n", s);
    }
  }

  // TODO: We should be able to remove "ignore noinit", but doing so causes
  // memory leaks in various places. Investigate why later and add noinit back
  // in when possible.
  pragma "string record"
  pragma "ignore noinit"
  pragma "no default functions"
  record string {
    var len: int = 0; // length of string in bytes
    var _size: int = 0; // size of the buffer we own
    var buff: bufferType = nil;
    var owned: bool = true;

    proc string(s: string, owned: bool = true) {
      this.owned = owned;
      const sRemote = s.locale.id != chpl_nodeID;
      if !owned && sRemote {
        // TODO: Should I just ignore the supplied value of owned instead of
        //       halting?
        halt("Must take ownership of a copy of a remote string");
      }
      var localBuff: bufferType = nil;
      const sLen = s.len;
      // Dont need to do anything if s is an empty string
      if sLen != 0 {
        localBuff = if sRemote
          then copyRemoteBuffer(s.locale.id, s.buff, sLen)
          else s.buff;
      }
      // We only need to copy when the buffer is local
      // TODO: reinitString does more checks then we actually need but is still
      // correct. Is it worth just doing the proper copy here?
      this.reinitString(localBuff, sLen, sLen+1, needToCopy=sRemote);
    }

    // This constructor can cause a leak if owned = false and needToCopy = true
    proc string(buff: bufferType, length: int, size: int,
                owned: bool = true, needToCopy: bool = true) {
      this.owned = owned;
      this.reinitString(buff, length, size, needToCopy);
    }

    proc ref ~string() {
      if owned && !this.isEmptyString() {
        on this {
          chpl_mem_free(this.buff);
        }
      }
    }

    // This is assumed to be called from this.locale
    proc ref reinitString(buf: bufferType, s_len: int, size: int,
                          needToCopy:bool = true) {
      if debugStrings then
        chpl_debug_string_print("in string.reinitString()");

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
            // We just allocaed a buffer, make sure to free it later
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

      if debugStrings then
        chpl_debug_string_print("leaving string.reinitString()");
    }

    inline proc c_str(): c_string {
      if debugStrings then
        chpl_debug_string_print("in .c_str()");

      if this.locale.id != chpl_nodeID then
        halt("Cannot call .c_str() on a remote string");

      if debugStrings then
        chpl_debug_string_print("leaving .c_str()");

      return this.buff:c_string;
    }

    inline proc length return len;
    //inline proc size return len;

    // Localize gets a version of this that is on the currently executing
    // locale. If this is already on the current locale we return a shallow
    // copy, otherwise a remote copy is performed.
    inline proc localize() : string {
        const ret: string = if this.locale.id != chpl_nodeID
          then this // assignment makes it local
          else new string(this, owned=false);
        return ret;
    }


    // Steals ownership of string.buff. This is unsafe and you probably don't
    // want to use it. I'd like to figure out how to get rid of it entirely.
    proc _steal_buffer() /*: bufferType*/ {
      const ret = this.buff;
      this.buff = nil;
      this.len = 0;
      this._size = 0;
      return ret;
    }

    // TODO: cant explicitly state return type right now due to a bug in the
    // compiler. Uncomment this function and others when possible.
    iter these() : string {
      for i in 1..this.len {
        yield this[i];
      }
    }

    proc this(i: int) : string {
      if i <= 0 || i > this.len then halt("index out of bounds of string");

      var ret: string;
      const newSize = chpl_mem_goodAllocSize(2);
      ret._size = max(chpl_string_min_alloc_size, newSize.safeCast(int));
      ret.len = 1;
      ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                                CHPL_RT_MD_STR_COPY_DATA): bufferType;
      ret.owned = true;

      const remoteThis = this.locale.id != chpl_nodeID;
      if remoteThis {
        chpl_string_comm_get(ret.buff, this.locale.id, this.buff, this.len.safeCast(size_t));
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
        const remoteThis = this.locale.id != chpl_nodeID;
        if remoteThis {
          // TODO: Could to an optimization here and only pull down the data
          // between r2.low and r2.high. Indexing for the copy below gets a bit
          // more complex when that is performed though.
          thisBuff = copyRemoteBuffer(this.locale.id, this.buff, this.len);
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

    // Returns a string containing the character at the given index of
    // the string, or an empty string if the index is out of bounds.
    inline proc substring(i: int) {
      compilerError("substring removed: use string[index]");
    }

    // Returns a string containing the string sliced by the given
    // range, or an empty string if the range does not overlap.
    inline proc substring(r: range) {
      compilerError("substring removed: use string[range]");
    }

    inline proc isEmptyString() : bool {
      return this.len == 0; // this should be enough of a check
    }

    //TODO: What is this for? nothing uses it...
    proc writeThis(f: Writer) {
      compilerWarning("not implemented: writeThis");
      /*
      if !this.isEmptyString() {
        if (this.locale.id != chpl_nodeID) {
          var tcs = copyRemoteBuffer(this.locale, this.buff, this.len);
          f.write(tcs);
          free_baseType(tcs);
        } else {
          f.write(this.buff);
        }
      }
      */
    }

    // TODO: could use a multi-pattern search or some variant when there are
    // multiple needles. Probably wouldnt be worth the overhead for small
    // needles though
    inline proc _startsEndsWith(needles: string ..., param fromLeft: bool) : bool {
      for needle in needles {
        if needle.isEmptyString() then return true;
        if needle.len > this.len then continue;

        const localNeedle: string = needle.localize();

        const needleR = 0:int..#localNeedle.len;
        if fromLeft {
          for n in needleR {
            if localNeedle.buff[n] != this.buff[n] then break;
            if n == localNeedle.len-1 then return true;
          }
        } else {
          const thisR = (this.len-localNeedle.len):int..this.len-1;
          for (n, t) in zip(needleR, thisR) {
            if localNeedle.buff[n] != this.buff[t] then break;
            if n == localNeedle.len-1 then return true;
          }
        }
      }
      return false;
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
      on this {
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
    iter split(maxsplit: int = -1, ignoreEmpty: bool = false) : string {
      for s in this.split(" ", maxsplit, ignoreEmpty) {
        yield s;
      }
    }

    iter split(sep: string, maxsplit: int = -1, ignoreEmpty: bool = false) : string {
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
            chunk = this;
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
            memmove(ret.buff+offset, this.buff, this.len.safeCast(size_t));
            offset += this.len;
          }
          var sLen = s.len;
          if sLen != 0 {
            if s.locale.id == chpl_nodeID {
              memmove(ret.buff+offset, s.buff, sLen.safeCast(size_t));
            } else {
              chpl_string_comm_get(ret.buff+offset, s.locale.id,
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

      on this {
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

      on this {
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

      on this {
        for i in 0..#this.len {
          const b = buff[i];
          // TODO: (escaped chars seem to break ascii...)
          if !((b == ascii(' ')) ||
               (b == 0x09) || // \t
               (b >= 0x0A && b <= 0x0D)) { // \n \v \f \r
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

      on this {
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

      on this {
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

      on this {
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

      on this {
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

      on this {
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
    if debugStrings then
      chpl_debug_string_print("in autoCopy()");

    // This pragma may be unnecessary.
    pragma "no auto destroy"
    var ret: string;
    const slen = s.len; // cache the remote copy of len
    if slen != 0 {
      if s.locale.id == chpl_nodeID {
        if debugStrings then
          chpl_debug_string_print("  local initCopy");
        ret.buff = chpl_mem_alloc(s._size.safeCast(size_t),
                                  CHPL_RT_MD_STR_COPY_DATA): bufferType;
        memmove(ret.buff, s.buff, s.len.safeCast(size_t));
        ret.buff[s.len] = 0;
      } else {
        if debugStrings then
          chpl_debug_string_print("  remote initCopy: "+s.locale.id:c_string);
        ret.buff = copyRemoteBuffer(s.locale.id, s.buff, slen);
      }
      ret.len = slen;
      ret._size = slen+1;
      ret.owned = true;
    }

    if debugStrings then
      chpl_debug_string_print("leaving autoCopy()");
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
    if debugStrings then
      chpl_debug_string_print("in initCopy()");

    var ret: string;
    const slen = s.len; // cache the remote copy of len
    if slen != 0 {
      if s.locale.id == chpl_nodeID {
        if debugStrings then
          chpl_debug_string_print("  local initCopy");
        ret.buff = chpl_mem_alloc(s._size.safeCast(size_t),
                                  CHPL_RT_MD_STR_COPY_DATA): bufferType;
        memmove(ret.buff, s.buff, s.len.safeCast(size_t));
        ret.buff[s.len] = 0;
      } else {
        if debugStrings then
          chpl_debug_string_print("  remote initCopy: "+s.locale.id:c_string);
        ret.buff = copyRemoteBuffer(s.locale.id, s.buff, slen);
      }
      ret.len = slen;
      ret._size = slen+1;
      ret.owned = true;
    }

    if debugStrings then
        chpl_debug_string_print("leaving initCopy()");
    return ret;
  }

  //
  // Assignment functions
  //
  proc =(ref lhs: string, rhs: string) {
    if debugStrings then
      chpl_debug_string_print("in proc =()");

    inline proc helpMe(ref lhs: string, rhs: string) {
      if rhs.locale.id == chpl_nodeID {
        if debugStrings then
          chpl_debug_string_print("  rhs local");
        lhs.reinitString(rhs.buff, rhs.len, rhs._size, needToCopy=true);
      } else {
        if debugStrings then
          chpl_debug_string_print("  rhs remote: "+rhs.locale.id:c_string);
        const len = rhs.len; // cache the remote copy of len
        var remote_buf:bufferType = nil;
        if len != 0 then
          remote_buf = copyRemoteBuffer(rhs.locale.id, rhs.buff, len);
        lhs.reinitString(remote_buf, len, len+1, needToCopy=false);
      }
    }

    if lhs.locale.id == chpl_nodeID then {
      if debugStrings then
        chpl_debug_string_print("  lhs local");
      helpMe(lhs, rhs);
    }
    else {
      if debugStrings then
        chpl_debug_string_print("  lhs remote: "+lhs.locale.id:c_string);
      on lhs do helpMe(lhs, rhs);
    }

    if debugStrings then
      chpl_debug_string_print("leaving proc =()");
  }

  proc =(ref lhs: string, rhs_c: c_string) {
    if debugStrings then
      chpl_debug_string_print("in proc =() c_string");

    // Make this some sort of local check once we have local types/vars
    if (rhs_c.locale.id != chpl_nodeID) || (lhs.locale.id != chpl_nodeID) then
      halt("Cannot assign a remote c_string to a string.");

    const len = rhs_c.length;
    const buff:bufferType = rhs_c:bufferType;
    lhs.reinitString(buff, len, len+1, needToCopy=true);

    if debugStrings then
      chpl_debug_string_print("leaving proc =() c_string");
  }

  //
  // Concatenation
  //
  proc +(s0: string, s1: string) {
    if debugStrings then
      chpl_debug_string_print("in proc +()");

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

    const s0remote = s0.locale.id != chpl_nodeID;
    if s0remote {
      chpl_string_comm_get(ret.buff, s0.locale.id,
                           s0.buff, s0len.safeCast(size_t));
    } else {
      memmove(ret.buff, s0.buff, s0len.safeCast(size_t));
    }

    const s1remote = s1.locale.id != chpl_nodeID;
    if s1remote {
      chpl_string_comm_get(ret.buff+s0len, s1.locale.id,
                           s1.buff, s1len.safeCast(size_t));
    } else {
      memmove(ret.buff+s0len, s1.buff, s1len.safeCast(size_t));
    }
    ret.buff[ret.len] = 0;

    if debugStrings then
      chpl_debug_string_print("leaving proc +()");
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

    const sRemote = s.locale.id != chpl_nodeID;
    if sRemote {
      chpl_string_comm_get(ret.buff, s.locale.id,
                           s.buff, sLen.safeCast(size_t));
    } else {
      memmove(ret.buff, s.buff, sLen.safeCast(size_t));
    }

    var iterations = n-1;
    var offset = sLen;
    for i in 1..iterations {
      memmove(ret.buff+offset, ret.buff, sLen.safeCast(size_t));
      offset += sLen;
    }
    ret.buff[ret.len] = 0;

    return ret;
  }

  /*
  inline proc _concat_helper(s: string, cs: c_string, param stringFirst: bool) {
    if debugStrings then
      chpl_debug_string_print("in proc +() string+c_string");

    if cs.locale.id != chpl_nodeID then
      halt("Cannot concatenate a remote c_string.");

    if cs == _defaultOf(c_string) then return s;
    const slen = s.len;
    if slen == 0 then return cs:string;

    var ret: string;
    ret.len = slen + cs.length;
    ret._size = ret.len+1;
    ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                              CHPL_RT_MD_STR_COPY_DATA): bufferType;
    ret.owned = true;

    const sremote = s.locale.id != chpl_nodeID;
    var sbuff = if sremote
                  then copyRemoteBuffer(s.locale.id, s.buff, slen)
                  else s.buff;

    if stringFirst {
      memmove(ret.buff, sbuff, slen.safeCast(size_t));
      memmove(ret.buff+slen, cs:bufferType, cs.length.safeCast(size_t));
    } else {
      memmove(ret.buff, cs:bufferType, cs.length.safeCast(size_t));
      memmove(ret.buff+cs.length, sbuff, slen.safeCast(size_t));
    }

    ret.buff[ret.len] = 0;

    if sremote then chpl_mem_free(sbuff);

    if debugStrings then
      chpl_debug_string_print("leaving proc +() string+c_string");
    return ret;
  }

  //TODO: figure out how to remove the concats between
  //      string and c_string[_copy]
  // promotion of c_string to string is masking this issue I think.
  proc +(s: string, cs: c_string) where isParam(cs) {
    //compilerWarning("adding c_string to string");
    return _concat_helper(s, cs, stringFirst=true);
  }

  proc +(cs: c_string, s: string) where isParam(cs) {
    //compilerWarning("adding string to c_string");
    return _concat_helper(s, cs, stringFirst=false);
  }

  // c_string_copy ones dont free right now... should they?
  proc +(s: string, /*ref*/ cs: c_string_copy) where isParam(cs) {
    //compilerWarning("adding c_string_copy to string");
    return _concat_helper(s, cs, stringFirst=true);
  }

  proc +(/*ref*/ cs: c_string_copy, s: string) where isParam(cs) {
    //compilerWarning("adding string to c_string_copy");
    return _concat_helper(s, cs, stringFirst=false);
  }*/

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

    on lhs {
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
          memmove(newBuff, lhs.buff, lhs.len.safeCast(size_t));
          lhs.buff = newBuff;
          lhs.owned = true;
        }

        lhs._size = newSize.safeCast(int);
      }
      const rhsRemote = rhs.locale.id != chpl_nodeID;
      if rhsRemote {
        chpl_string_comm_get(lhs.buff+lhs.len, rhs.locale.id,
                              rhs.buff, rhsLen.safeCast(size_t));
      } else {
        memmove(lhs.buff+lhs.len, rhs.buff, rhsLen.safeCast(size_t));
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
    var idx: int = 0;
    while (idx < a.len && idx < b.len) {
      if a.buff[idx] != b.buff[idx] then return a.buff[idx]:int - b.buff[idx];
      idx += 1;
    }
    // What if the next character in the longer buffer is 0x0?
    // I just return +1 or -1 to avoid that question.
    if (idx < a.len) then return 1;
    if (idx < b.len) then return -1;
    return 0;
  }

  proc ==(a: string, b: string) : bool {
    inline proc doEq(a: string, b:string) {
      // TODO: is it better to have these outside of the do* fns?
      //       probably would be 2 extra gets worst case, but avoids doing a
      //       local copy if b is remote
      if a.len != b.len then return false;
      return _strcmp(a, b) == 0;
    }

    if a.locale.id == b.locale.id {
      var ret: bool = false;
      on a do ret = doEq(a, b);
      return ret;
    } else {
      var localA: string = a.localize();
      var localB: string = b.localize();

      return doEq(localA, localB);
    }
  }

  inline proc !=(a: string, b: string) : bool {
    return !(a == b);
  }

  inline proc <(a: string, b: string) : bool {
    inline proc doLt(a: string, b:string) {
      return _strcmp(a, b) < 0;
    }

    if a.locale.id == b.locale.id {
      var ret: bool = false;
      on a do ret = doLt(a, b);
      return ret;
    } else {
      var localA: string = a.localize();
      var localB: string = b.localize();

      return doLt(localA, localB);
    }
  }

  inline proc >(a: string, b: string) : bool {
    inline proc doGt(a: string, b:string) {
      return _strcmp(a, b) > 0;
    }

    if a.locale.id == b.locale.id {
      var ret: bool = false;
      on a do ret = doGt(a, b);
      return ret;
    } else {
      var localA: string = a.localize();
      var localB: string = b.localize();

      return doGt(localA, localB);
    }
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
  inline proc _byte_isUpper(b: uint(8)) : bool {
    return b >= ascii('A') && b <= ascii('Z');
  }

  inline proc _byte_isLower(b: uint(8)) : bool {
    return b >= ascii('a') && b <= ascii('z');
  }

  inline proc _byte_isAlpha(b: uint(8)) : bool {
    return b >= ascii('A')  && b <= ascii('z');
  }

  inline proc _byte_isDigit(b: uint(8)) : bool {
    return b >= ascii('0')  && b <= ascii('9');
  }


  //
  // ascii
  // TODO: replace with ordinal()
  //
  inline proc ascii(a: string) : int(32) {
    if a.isEmptyString() then return 0;

    return a.buff[0]:int(32);
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

  // TODO:7-29-2015: check if these can be removed
  // :(
  inline proc _cast(type t, cs: c_string) where t == bufferType {
    return __primitive("cast", t, cs);
  }

  // :( (for debugStrings)
  inline proc _cast(type t, x) where t:c_string && x.type:bufferType {
    return __primitive("cast", t, x);
  }

  // :(
  inline proc _cast(type t, cs: c_string_copy) where t == bufferType {
    return __primitive("cast", t, cs);
  }

  // Cast from c_string to string
  // TODO: I don't like this, but cant get rid of it without doing something
  //       with making dtString the type for literals I think...
  proc _cast(type t, cs: c_string) where t == string {
    if debugStrings then
      chpl_debug_string_print("in _cast() c_string->string");

    if cs.locale.id != chpl_nodeID then
      halt("Cannot cast a remote c_string to string.");

    var ret: string;
    ret.len = cs.length;
    ret._size = ret.len+1;
    ret.buff = if ret.len > 0
      then __primitive("string_copy", cs): bufferType
      else nil;
    ret.owned = true;

    if debugStrings then
      chpl_debug_string_print("leaving _cast() c_string->string");
    return ret;
  }

  //
  // Developer Extras
  //

  proc chpldev_refToString(ref arg) : string {
    // print out the address of class references as well
    proc chpldev_classToString(x: object) : string
      return " (class = " + __primitive("ref to string", x) + ")";
    proc chpldev_classToString(x) : string return "";

    return __primitive("ref to string", arg) + chpldev_classToString(arg);
  }
}
