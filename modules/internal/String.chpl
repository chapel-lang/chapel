/*
    // use reinitString directly if that is not the case
    // buff must be local
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
 * - An empty string is represented by the default bufferType value
 *   which is always considered to be a string literal.  It will never
 *   be freed or reference counted.  Another option would be to use a
 *   special NULL extern symbol.  Unfortunately, since _defaultOf()
 *   currently does not work for types with "ignore noinit", we'd
 *   still have to handle the default bufferType value, so it would not
 *   be worth it to use NULL.  If the situation changes, it might be
 *   worth reconsidering (also taking into consideration the runtime
 *   functions that back this).
 *
 * - It is assumed the bufferType is a local-only type, so we never
 *   make a remote copy of one passed in by the user, though remote
 *   copies are made of internal bufferType variables.
 *
 */

module BaseStringType {
  use CString;
  use SysCTypes;

  type bufferType = c_ptr(uint(8));
  param bufferTypeString: c_string = "c_ptr(uint(8))";
  param min_alloc_size: int = 16;
  // Growth factor to use when extending the buffer for appends
  config param chpl_stringGrowthFactor = 1.5;

  extern type chpl_mem_descInt_t;

  // We use this as a shortcut to get at here.id without actually constructing
  // a locale object. Used when determining if we should make a remote transfer.
  // This extern is also defined in ChapelLocale, but if it isn't defined here
  // as well we will get a duplicate symbol error. I believe this is due to
  // module load ordering issues.
  // TODO: make a proc in ChapelLocale that returns this rather than using the extern
  extern var chpl_nodeID: chpl_nodeID_t;

  // TODO: hook into chpl_here_alloc and friends somehow
  // The runtime exits on errors for these
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

  pragma "string record"
  pragma "ignore noinit"
  pragma "no default functions"
  record string {
    var len: int = 0; // length of string in bytes
    var _size: int = 0; // size of the buffer we own
    var buff: bufferType = nil;
    // I can almost get away without this, but other things get verbose.
    // This causes us to get padded out to a word boundary :(
    // TODO: change to const when const checking for auto/initCopy is fixed
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

      // If the this.buff is longer than buf, then reuse the buffer
      if s_len != 0 {
        if needToCopy {
          if s_len+1 > this._size {
            // free the old buffer
            if this.owned && !this.isEmptyString() then
              on this do chpl_mem_free(this.buff);
            // allocate a new buffer
            this.buff = chpl_mem_alloc((s_len+1).safeCast(size_t),
                                       CHPL_RT_MD_STR_COPY_DATA):bufferType;
            this.buff[s_len] = 0;
            this._size = s_len+1;
          }
          memmove(this.buff, buf, s_len.safeCast(size_t));
        } else {
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

    // Steals ownership of string.buff. This is unsafe and you probably don't
    // want to use it. I'd like to figure out how to get rid of it entirely.
    proc _steal_buffer() : bufferType {
      const ret = this.buff;
      this.buff = nil;
      this.len = 0;
      this._size = 0;
      return ret;
    }

    iter these() : string {
      for i in 1..this.len {
        yield this[i];
      }
    }

    proc this(i: int) : string {
      var ret: string;
      if i <= 0 || i > this.len then halt("index out of bounds of string");

      ret._size = min_alloc_size;
      ret.len = 1;
      ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                                CHPL_RT_MD_STR_COPY_DATA): bufferType;

      var remoteThis = this.locale.id != chpl_nodeID;
      if remoteThis {
        chpl_string_comm_get(ret.buff, this.locale.id, this.buff, this.len.safeCast(size_t));
      } else {
        ret.buff[0] = this.buff[i-1];
      }
      ret.buff[1] = 0;

      return ret;
    }

    // TODO: I wasn't very good about caching variables locally in this one.
    proc this(r: range(?)) : string {
      var ret: string;
      if this.isEmptyString() then return ret;

      //TODO: halt()s should use string.writef at some point.
      if r.hasLowBound() {
        if r.low <= 0 then
          halt("range out of bounds of string");
          //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
      }
      if r.hasHighBound() {
        if r.high < 0 || r.high:int > this.len then
          halt("range out of bounds of string");
          //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
      }

      var r2 = r[1:r.idxType..#(this.len:r.idxType)];
      if r2.size <= 0 {
        halt("tring to slice a string with a range of size 0");
        //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
      } else {
        ret.len = r2.size:int;
      }
      ret._size = if ret.len+1 > min_alloc_size then ret.len+1 else min_alloc_size;
      ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                                CHPL_RT_MD_STR_COPY_DATA): bufferType;

      var thisBuff: bufferType;
      var remoteThis = this.locale.id != chpl_nodeID;
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

    inline proc isEmptyString() {
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
    proc startsWith(needles: string ...) : bool {
      for needle in needles {
        if needle.isEmptyString() then return true;
        if needle.len > this.len then continue;

        for i in 0:int..#needle.len {
          if needle.buff[i] != this.buff[i] then break;
          if i == needle.len-1 then return true;
        }
      }
      return false;
    }

    // Returns the index of the first occurrence of a substring within a
    // string or 0 if the substring is not in the string.
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    proc find(needle: string) : int {
      const nlen = needle.len;
      const thisLen = this.len;
      if nlen == 0 then return 1; //TODO: should this always be 0?
      if thisLen == 0 then return 0;
      if nlen > thisLen then return 0;

      var ret: int = 0;
      // needle.len is <= than this.len, so go to the home locale
      on this {
        var localNeedle: string = if needle.locale.id != chpl_nodeID
        then needle // assignment makes it local
        else new string(needle, owned=false);

        const lastPossible = (this.len-localNeedle.len)+1;
        for i in 0..#lastPossible {
          for j in 0..#localNeedle.len {
            if this.buff[i+j] != localNeedle.buff[j] then break;
            if j == localNeedle.len-1 then ret = i+1;
          }
          if ret != 0 then break;
        }
      }
      return ret;
    }
  }

  // We'd like this to be by ref, but doing so leads to an internal
  // compiler error.  See
  // $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
  pragma "donor fn"
  pragma "auto copy fn"
  proc chpl__autoCopy(s: string) {
    if debugStrings then
      chpl_debug_string_print("in autoCopy()");

    pragma "no auto destroy"
    var ret: string;

    if !s.isEmptyString() {
      ret.buff = chpl_mem_alloc((s.len+1).safeCast(size_t),
                                CHPL_RT_MD_STR_COPY_DATA): bufferType;
      memmove(ret.buff, s.buff, s.len.safeCast(size_t));
      ret.len = s.len;
      ret.owned = true; // I now own my data.
      ret._size = s.len+1;
      ret.buff[ret.len] = 0;
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
        ret.buff = chpl_mem_alloc((s.len+1).safeCast(size_t),
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

    const hereId = chpl_nodeID;
    // Make this some sort of local check once we have local types/vars
    if (rhs_c.locale.id != hereId) || (lhs.locale.id != hereId) then
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
    ret._size = ret.len+1;
    ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                              CHPL_RT_MD_STR_COPY_DATA): bufferType;

    const hereId = chpl_nodeID;
    const s0remote = s0.locale.id != hereId;
    if s0remote {
      chpl_string_comm_get(ret.buff, s0.locale.id,
                           s0.buff, s0len.safeCast(size_t));
    } else {
      memmove(ret.buff, s0.buff, s0len.safeCast(size_t));
    }

    const s1remote = s1.locale.id != hereId;
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

  inline proc _concat_helper(s: string, cs: c_string, param stringFirst: bool) {
    if debugStrings then
      chpl_debug_string_print("in proc +() string+c_string");

    const hereId = chpl_nodeID;
    if cs.locale.id != hereId then
      halt("Cannot concatenate a remote c_string.");

    if cs == _defaultOf(c_string) then return s;
    const slen = s.len;
    if slen == 0 then return cs:string;

    var ret: string;
    ret.len = slen + cs.length;
    ret._size = ret.len+1;
    ret.buff = chpl_mem_alloc(ret._size.safeCast(size_t),
                              CHPL_RT_MD_STR_COPY_DATA): bufferType;
    const sremote = s.locale.id != hereId;
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
  // Append
  //
  proc +=(ref lhs: string, rhs: string) : void {
    //TODO: check for overflow
    on lhs {
      const rhsLen = rhs.len;
      const new_length = lhs.len+rhsLen;
      if lhs._size < (new_length) {
        var new_size = max(new_length+1, (lhs.len * chpl_stringGrowthFactor):int);
        lhs.buff = chpl_mem_realloc(lhs.buff, (new_size).safeCast(size_t),
                                    CHPL_RT_MD_STR_COPY_DATA):c_ptr(uint(8));
        lhs._size = new_size;
      }
      const hereId = chpl_nodeID;
      const rhsRemote = rhs.locale.id != hereId;
      if rhsRemote {
        chpl_string_comm_get(lhs.buff+lhs.len, rhs.locale.id,
                            rhs.buff, rhsLen.safeCast(size_t));
      } else {
        memmove(lhs.buff+lhs.len, rhs.buff, rhsLen.safeCast(size_t));
      }
      lhs.len = lhs.len+rhsLen;
      lhs.buff[new_length] = 0;
    }
  }

  //
  // Relational operators
  // TODO: all relational ops other than == and != are broken for unicode
  // TODO: It may be faster to work on a.locale or b.locale if a or b is large
  //
  // When strings are compared, they compare equal up to the point the
  // characters in the string differ.   At that point, if the encoding of the
  // next character is numerically smaller it will compare less, otherwise
  // greater.  If the encoding of one character uses fewer bits, the smaller
  // one is zero-extended to match the langth of the longer one.
  // For purposes of comparison, if one comparand is shorter than the
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
      if a.len != b.len then return false;
      return _strcmp(a, b) == 0;
    }
    if a.locale.id == b.locale.id {
      return doEq(a, b);
    } else {
      // TODO: any away to do something like this?
      //       var localA = makeLocal(a);
      //       var localB = makeLocal(b);
      var localA: string;
      if a.locale.id != chpl_nodeID {
        localA = a; // assignment makes it local
      } else {
        localA = new string(a.buff, a.len, a._size,
                            owned = false, needToCopy=false);
      }

      var localB: string;
      if b.locale.id != chpl_nodeID {
        localB = b; // assignment makes it local
      } else {
        localB = new string(b.buff, b.len, b._size,
                            owned = false, needToCopy=false);
      }

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
      return doLt(a, b);
    } else {
      var localA: string;
      if a.locale.id != chpl_nodeID {
        localA = a; // assignment makes it local
      } else {
        localA = new string(a.buff, a.len, a._size,
                            owned = false, needToCopy=false);
      }

      var localB: string;
      if b.locale.id != chpl_nodeID {
        localB = b; // assignment makes it local
      } else {
        localB = new string(b.buff, b.len, b._size,
                            owned = false, needToCopy=false);
      }

      return doLt(localA, localB);
    }
  }

  inline proc >(a: string, b: string) : bool {
    inline proc doGt(a: string, b:string) {
      return _strcmp(a, b) > 0;
    }
    if a.locale.id == b.locale.id {
      return doGt(a, b);
    } else {
      var localA: string;
      if a.locale.id != chpl_nodeID {
        localA = a; // assignment makes it local
      } else {
        localA = new string(a.buff, a.len, a._size,
                            owned = false, needToCopy=false);
      }

      var localB: string;
      if b.locale.id != chpl_nodeID {
        localB = b; // assignment makes it local
      } else {
        localB = new string(b.buff, b.len, b._size,
                            owned = false, needToCopy=false);
      }

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
  inline proc _cast(type t, cs: c_string) where t == string {
    if debugStrings then
      chpl_debug_string_print("in _cast() c_string->string");

    if cs.locale.id != chpl_nodeID then
      halt("Cannot cast a remote c_string to string.");

    var ret: string;
    ret.len = cs.length;
    ret._size = ret.len+1;
    if !ret.isEmptyString() then ret.buff = __primitive("string_copy", cs): bufferType;

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
