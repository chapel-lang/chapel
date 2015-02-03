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

pragma "no use ChapelStandard"

/*
 * NOTES:
 *
 * - An empty string is represented by the default baseType value
 *   which is always considered to be a string literal.  It will never
 *   be freed or reference counted.  Another option would be to use a
 *   special NULL extern symbol.  Unfortunately, since _defaultOf()
 *   currently does not work for types with "ignore noinit", we'd
 *   still have to handle the default baseType value, so it would not
 *   be worth it to use NULL.  If the situation changes, it might be
 *   worth reconsidering (also taking into consideration the runtime
 *   functions that back this).
 *
 * - Not all operations support interaction with the baseType, e.g.,
 *   relational operations.  I chose to implement the ones I thought
 *   were commonly used like concatenation and assignment.  When this
 *   becomes the default, the compiler should automatically coerce the
 *   rest.
 *
 * - It is assumed the the baseType is a local-only type, so we never
 *   make a remote copy of one passed in by the user, though remote
 *   copies are made of internal baseType variables.
 *
 */

module BaseStringType {
  use CString;

  type bufferType = c_ptr(uint(8));
  param bufferTypeString = "c_ptr(uint(8))":c_string;
  param min_alloc_size: uint = 16;

  extern type chpl_mem_descInt_t;

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

  extern const CHPL_RT_MD_STRING_COPY_REMOTE: chpl_mem_descInt_t;
  extern const CHPL_RT_MD_STRING_COPY_DATA: chpl_mem_descInt_t;

  // TODO: maybe remove this one, mostly used as a helper for me to make sure I
  // get the signature right.
  inline proc chpl_string_comm_get(dest: bufferType, src_loc_id: int(64),
                                   src_addr: bufferType, len: size_t) {
    __primitive("chpl_comm_get", dest, src_loc_id, src_addr, len);
  }

  proc copyRemoteBuffer(src_loc_id: int(64), src_addr: bufferType, len: size_t): bufferType {
      const dest = chpl_mem_alloc(len+1, CHPL_RT_MD_STRING_COPY_REMOTE): bufferType;
      chpl_string_comm_get(dest, src_loc_id, src_addr, len);
      dest[len+1] = 0;
      return dest;
  }

  // TODO: I think this could be better
  inline proc makeLocal(x: string) : string {
    if x.home.id != here.id {
      var localX: string = x;
      return localX;
    }
    return x;
  }

  extern proc memmove(destination: c_ptr, const source: c_ptr, num: size_t);

  // pointer arithmatic used for strings
  inline proc +(a: c_ptr, b: int(?w)) return __primitive("+", a, b);
  inline proc +(a: c_ptr, b: uint(?w)) return __primitive("+", a, b);

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
    var base: bufferType;
    var len: uint; // length of string in bytes
    var _size: uint; // size of the buffer we own
    var home: locale; // this does not change through assignment

    proc string() {
      if debugStrings then
        chpl_debug_string_print("in string()");

      this.home = here;

      if debugStrings then
        chpl_debug_string_print("leaving string()");
    }

    proc ref ~string() {
      if debugStrings then
        chpl_debug_string_print("in ~string()");

      if !this.isEmptyString() {
        if debugStrings then
          chpl_debug_string_print("  freeing base: "+this.base:c_string);
        on this.home {
          chpl_mem_free(this.base);
        }
      }

      if debugStrings then
        chpl_debug_string_print("leaving ~string()");
    }

    // This is assumed to be called from this.home
    proc ref reinitString(buf: bufferType, s_len:uint) {
      assert(buf.locale.id == here.id);
      assert(this.home.id == here.id);
      if debugStrings then
        chpl_debug_string_print("in string.reinitString()");

      if this.isEmptyString() {
        if (s_len == 0) || (buf == nil) then return; // nothing to do
      }

      // If the this.base is longer than buf, then reuse the buffer
      if s_len != 0 {
        if s_len+1 > this._size {
          // free the old buffer
          if this.len != 0 then
            on home do chpl_mem_free(this.base);
          // allocate a new buffer
          this.base = chpl_mem_alloc(s_len+1, CHPL_RT_MD_STRING_COPY_DATA):bufferType;
          this._size = s_len+1;
        }
        memmove(this.base, buf, s_len);
        this.base[s_len+1] = 0;
      } else {
        // free the old buffer
        if this.len != 0 then chpl_mem_free(this.base);
        this.base = nil;
      }

      this.len = s_len;

      if debugStrings then
        chpl_debug_string_print("leaving string.reinitString()");
    }

    inline proc c_str(): c_string {
      if debugStrings then
        chpl_debug_string_print("in .c_str()");

      if this.home.id != here.id then
        halt("Cannot call .c_str() on a remote string");

      if debugStrings then
        chpl_debug_string_print("leaving .c_str()");

      return this.base:c_string;
    }

    inline proc length return len;
    //inline proc size return len;

    // Steals ownership of string.base. This is unsafe and you probably don't
    // want to use it. I'd like to figure out how to get rid of it entirely.
    proc _steal_base() : bufferType {
      const ret = this.base;
      this.base = nil;
      this.len = 0;
      this._size = 0;
      return ret;
    }

    proc this(i: uint) {
      var ret: string;
      if i == 0 || i > this.len then return ret;

      ret._size = min_alloc_size;
      ret.len = 1;
      ret.base = chpl_mem_alloc(ret._size, CHPL_RT_MD_STRING_COPY_DATA): bufferType;

      var remoteThis = this.home.id != here.id;
      if remoteThis {
        chpl_string_comm_get(ret.base, this.home.id, this.base, this.len);
      } else {
        ret.base[0] = this.base[i];
      }
      ret.base[1] = 0;

      return ret;
    }

    // TODO: I wasn't very good about caching variables locally in this one.
    proc this(r: range(?)) {
      var ret: string;
      if this.len == 0 then return ret;

      //TODO: halt()s should use string.writef at some point.
      if r.hasLowBound() {
        if r.low <= 0 then
          halt("range out of bounds of string");
          //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
      }
      if r.hasHighBound() {
        if r.high < 0 || r.high:uint > this.len then
          halt("range out of bounds of string");
          //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
      }

      var r2 = r[1:r.idxType..#this.len];
      if r2.size <= 0 {
        halt("range out of bounds of string");
        //halt("range %t out of bounds of string %t".writef(r, 1..this.len));
      } else {
        ret.len = r2.size:uint;
      }
      ret._size = if ret.len+1 > min_alloc_size then ret.len+1 else min_alloc_size;
      ret.base = chpl_mem_alloc(ret._size, CHPL_RT_MD_STRING_COPY_DATA): bufferType;

      var thisBase: bufferType;
      var remoteThis = this.home.id != here.id;
      if remoteThis {
        // TODO: Could to an optimization here and only pull down the data
        // between r2.low and r2.high. Indexing for the copy below gets a bit
        // more complex when that is performed though.
        thisBase = copyRemoteBuffer(this.home.id, this.base, this.len);
      } else {
        thisBase = this.base;
      }

      for (r2_i, i) in zip(r2, 0..) {
        ret.base[i] = thisBase[r2_i-1];
      }
      ret.base[ret.len+1] = 0;

      if remoteThis then chpl_mem_free(thisBase);

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

    proc writeThis(f: Writer) {
      compilerWarning("not implemented: writeThis");
      /*
      if !this.isEmptyString() {
        if (this.home.id != here.id) {
          var tcs = copyRemoteBuffer(this.home, this.base, this.len);
          f.write(tcs);
          free_baseType(tcs);
        } else {
          f.write(this.base);
        }
      }
      */
    }

    // TODO: could use a multi-pattern search or some variant when there are
    // multiple needles
    proc startsWith(needles: string ...) : bool {
      for needle in needles {
        if needle.len == 0 then return true;
        if needle.len > this.len then continue;

        for i in 0..needle.len {
          if needle.base[i] != this.base[i] then break;
          if i == needle.len then return true;
        }
      }
      return false;
    }

    // Returns the index of the first occurrence of a substring within a
    // string or 0 if the substring is not in the string.
    //TODO: this could be a much better string search (Boyer-Moore/whatever)
    inline proc find(s: string) : uint {
      if s.len == 0 then return 1; //TODO: should this always be 0?
      if this.len == 0 then return 0;
      var ret: uint = 0;
      // Assume s.base is shorter than this.base, so go to the home locale
      on this.home {
        var sLocal = makeLocal(s);
        for i in 0:uint..#this.len {
          for j in 0:uint..#sLocal.len {
            if this.base[i] != sLocal.base[j] then break;
            if j == sLocal.len-1 then ret = i+1;
          }
          if ret != 0 then break;
        }
      }
      return ret;
    }
  }

  pragma "donor fn"
  pragma "auto copy fn"
    // We'd like this to be by ref, but doing so leads to an internal
    // compiler error.  See
    // $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
    /*inline*/ proc chpl__autoCopy(/*ref*/ s: string) {
    if debugStrings then
      chpl_debug_string_print("in autoCopy()");

    pragma "no auto destroy"
    var ret: string;
    if !s.isEmptyString() {
      ret.home = s.home;
      ret.base = chpl_mem_alloc(s.len+1, CHPL_RT_MD_STRING_COPY_DATA): bufferType;
      memmove(ret.base, s.base, s.len);
      ret.len = s.len;
      ret._size = s.len+1;
      ret.base[ret.len+1] = 0;
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
   */
  pragma "init copy fn"
  /*inline*/ proc chpl__initCopy(ref s: string) {
    if debugStrings then
      chpl_debug_string_print("in initCopy()");

    var ret: string;
    if !s.isEmptyString() {
      const slen = s.len; // cache the remote copy of len
      if s.home.id == here.id {
        if debugStrings then
          chpl_debug_string_print("  local initCopy");
        ret.base = chpl_mem_alloc(s.len+1, CHPL_RT_MD_STRING_COPY_DATA): bufferType;
        memmove(ret.base, s.base, s.len);
      } else {
        if debugStrings then
          chpl_debug_string_print("  remote initCopy: "+s.home.id:c_string);
        ret.base = copyRemoteBuffer(s.home.id, s.base, slen);
      }
      ret.len = slen;
      ret._size = slen;
      ret.base[ret.len+1] = 0;
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
      if rhs.home.id == here.id {
        if debugStrings then
          chpl_debug_string_print("  rhs local");
        lhs.reinitString(rhs.base, rhs.len);
      } else {
        if debugStrings then
          chpl_debug_string_print("  rhs remote: "+rhs.home.id:c_string);
        const len = rhs.len; // cache the remote copy of len
        // TODO: reuse the base from copyRemoteBuffer()
        var remote_buf:bufferType = nil;
        if len != 0 then
          remote_buf = copyRemoteBuffer(rhs.home.id, rhs.base, len);
        lhs.reinitString(remote_buf, len);
        if len != 0 then chpl_mem_free(remote_buf);
      }
    }

    if lhs.home.id == here.id then {
      if debugStrings then
        chpl_debug_string_print("  lhs local");
      helpMe(lhs, rhs);
    }
    else {
      if debugStrings then
        chpl_debug_string_print("  lhs remote: "+lhs.home.id:c_string);
      on lhs do helpMe(lhs, rhs);
    }

    if debugStrings then
      chpl_debug_string_print("leaving proc =()");
  }

  proc =(ref lhs: string, rhs_c: c_string) {
    if debugStrings then
      chpl_debug_string_print("in proc =() c_string");

    const hereId = here.id;
    // Make this some sort of local check once we have local types/vars
    if (rhs_c.locale.id != hereId) || (lhs.home.id != hereId) then
      halt("Cannot assign a remote c_string to a string.");
    const len = rhs_c.length;
    const buff:bufferType = rhs_c:bufferType;
    lhs.reinitString(buff, len);

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
    ret.base = chpl_mem_alloc(ret._size, CHPL_RT_MD_STRING_COPY_DATA): bufferType;

    const s0remote = s0.home.id != here.id;
    if s0remote {
      chpl_string_comm_get(ret.base, s0.home.id, s0.base, s0len);
    } else {
      memmove(ret.base, s0.base, s0len);
    }

    const s1remote = s1.home.id != here.id;
    if s1remote {
      chpl_string_comm_get(ret.base+s0len, s1.home.id, s1.base, s1len);
    } else {
      memmove(ret.base+s0len, s1.base, s1len);
    }
    ret.base[ret.len+1] = 0;

    if debugStrings then
      chpl_debug_string_print("leaving proc +()");
    return ret;
  }

  inline proc _concat_helper(s: string, cs: c_string, param stringFirst: bool) {
    if debugStrings then
      chpl_debug_string_print("in proc +() string+c_string");

    if cs.locale.id != here.id then
      halt("Cannot concatenate a remote c_string.");

    if cs == _defaultOf(c_string) then return s;
    if s.isEmptyString() then return cs:string;

    var ret: string;
    const slen = s.len;
    ret.len = slen + cs.length;
    ret._size = ret.len+1;
    ret.base = chpl_mem_alloc(ret._size, CHPL_RT_MD_STRING_COPY_DATA): bufferType;
    const sremote = s.home.id != here.id;
    var sbase = if sremote
                  then copyRemoteBuffer(s.home.id, s.base, slen)
                  else s.base;

    if stringFirst {
      memmove(ret.base, sbase, slen);
      memmove(ret.base+slen, cs:bufferType, cs.length);
    } else {
      memmove(ret.base, cs:bufferType, cs.length);
      memmove(ret.base+cs.length, sbase, slen);
    }

    ret.base[ret.len+1] = 0;

    if sremote then chpl_mem_free(sbase);

    if debugStrings then
      chpl_debug_string_print("leaving proc +() string+c_string");
    return ret;
  }

  //TODO: figure out how to remove the concats between
  //      string and c_string[_copy]
  //      Not as bad with the isParam clause
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

  //
  // Concatenation with other types is done by casting to string
  //
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
  // Relational operators
  // TODO: all relational ops other than == and != are broken for unicode
  // TODO: It may be faster to work on a.home or b.home if a or b is large
  //
  inline proc _strcmp(a: string, b:string) : int {
    // Assumes a and b are on same locale and not empty
    var idx: uint = 0;
    while (idx < a.len) && (idx < b.len) {
      if a.base[idx] != b.base[idx] then return a.base[idx]:int - b.base[idx];
      idx += 1;
    }
    return 0;
  }

  proc ==(a: string, b: string) : bool {
    inline proc doEq(a: string, b:string) {
      if a.len != b.len then return false;
      if a.len == 0 then return true;
      return _strcmp(a, b) == 0;
    }
    if a.home.id == b.home.id {
      return doEq(a, b);
    } else {
      var localA = makeLocal(a);
      var localB = makeLocal(b);
      return doEq(localA, localB);
    }
  }

  inline proc !=(a: string, b: string) : bool {
    return !(a == b);
  }

  inline proc <(a: string, b: string) : bool {
    inline proc doLt(a: string, b:string) {
      if b.len == 0 then return false;
      if a.len == 0 then return true;
      return _strcmp(a, b) < 0;
    }
    if a.home.id == b.home.id {
      return doLt(a, b);
    } else {
      var localA = makeLocal(a);
      var localB = makeLocal(b);
      return doLt(localA, localB);
    }
  }

  inline proc >(a: string, b: string) : bool {
    inline proc doGt(a: string, b:string) {
      if a.len == 0 then return false;
      if b.len == 0 then return true;
      return _strcmp(a, b) > 0;
    }
    if a.home.id == b.home.id {
      return doGt(a, b);
    } else {
      var localA = makeLocal(a);
      var localB = makeLocal(b);
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
  // Casts (casts to / from other primitive types are in StringCasts)
  //
  // TODO: I want to break all of these casts from string to T out into
  // T.parse(string), but we dont support methods on types yet. Ideally they
  // would use a tagged union retval as well.

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

  // Cast from baseType to string
  // TODO: I dont like this, but cant get rid of it without doing something
  //       with making dtString the type for literals I think...
  inline proc _cast(type t, cs: c_string) where t == string {
    if debugStrings then
      chpl_debug_string_print("in _cast() c_string->string");

    if cs.locale.id != here.id then
      halt("Cannot cast a remote c_string to string.");

    var ret: string;
    ret.len = cs.length;
    ret._size = ret.len;
    if ret.len != 0 then ret.base = __primitive("string_copy", cs): bufferType;

    if debugStrings then
      chpl_debug_string_print("leaving _cast() c_string->string");
    return ret;
  }
}
