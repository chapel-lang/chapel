// pragma "no use ChapelStandard"

/*
 * RANDOM NOTES:
 *
 * - Should have primitives like string_copy and string_concat take
 *   option (or maybe required) length arguments to avoid the extra
 *   call to strlen().
 * 
 * - The default c_string value is always considered to be a string
 *   literal and will never be freed or reference counted.  NOTE: TEST
 *   REASSIGNMENT TO "".
 *
 * - Consider storing max size in the string record to enable more
 *   control of buffer reuse.
 *
 */

module StringExterns {
  pragma "insert line file info"
  extern proc stringMove(dest: c_string, src: c_string, len: int): c_string;
  pragma "insert line file info"
  extern proc remoteStringCopy(src_loc: int, src_addr: c_string, len: int): c_string;

  config const noisyConstructor = false;
}


// Chapel Strings
module NewString {
  use CString;
  use StringExterns;

  class string_refcnt {
    var val: atomic_refcnt;
    inline proc inc(cnt=1) { val.inc(); }
    inline proc dec(cnt=1) return val.dec();
    inline proc read(cnt=1) return val._cnt.read();
  }

  // pragma "string record"
  pragma "ignore noinit"
  pragma "no default functions"
  record sungeun_string {
    var base: c_string = "";
    var len: int = 0;
    var home: locale;
    var refCnt: string_refcnt;
    var aliasRefCnt: string_refcnt;

    proc sungeun_string() {
      if noisyConstructor then writeln("in string()");
      this.home = here;
      this.refCnt = new string_refcnt();
      this.aliasRefCnt = this.refCnt;
      if noisyConstructor then writeln("leaving string()");
    }

    proc ref ~sungeun_string() {
      if noisyConstructor then writeln("in ~string()");
      if this.isDefaultValue() {
        delete this.refCnt;
      } else {
        const refs =  decRefCnt();
        if refs < 0 then halt("string reference count is negative!");
        if refs == 0 then {
          if noisyConstructor then writeln("  freeing base: ", this.base);
          on this.home {
            chpl_free_c_string(this.base);
            freeRefCnt();
          }
        } else {
          if noisyConstructor then writeln("  not freeing base: ", this.base);
          if isAlias then delete this.refCnt;
        }
      }
      if noisyConstructor then writeln("leaving ~string()");
    }

    proc ref reinitString(s: c_string, slen:int =-1) {
      if noisyConstructor then writeln("in string.reinitString()");
      const new_len = if slen == -1 then __primitive("string_length", s)
                      else slen;
      if this.isDefaultValue() then this.incRefCntNoAlias();
      // If the this.base is longer than s, then reuse the buffer
      if new_len != 0 {
        if new_len < this.len {
          // reuse the buffer
          this.base = stringMove(this.base, s, new_len);
        } else {
          // free the old buffer
          if this.len != 0 then
            on home do chpl_free_c_string(this.base);
          // allocate a new buffer
          this.base = stringMove(_defaultOf(c_string), s, new_len);
        }
      } else {
        // free the old buffer
        if this.len != 0 then
          on home do chpl_free_c_string(this.base);
        this.base = _defaultOf(c_string); // empty string are always literals
      }
      this.len = new_len;
      this.home = here;
      if noisyConstructor then writeln("leaving string.reinitString()");
    }

    inline proc c_str() {
      if noisyConstructor then writeln("in .c_str()");
      if this.home.id != here.id then
        halt("Cannot call .c_str() on a remote string");
      if noisyConstructor then writeln("leaving .c_str()");
      return this.base;
    }

    inline proc length return len;
    inline proc size return len;

    // Returns a string containing the character at the given index of
    // the string, or an empty string if the index is out of bounds.
    inline proc substring(i: int) {
      if this.isDefaultValue() || i<=0 then return new sungeun_string();
      const sremote = this.home.id != here.id;
      const sbase = if sremote
                    then remoteStringCopy(this.home.id, this.base, this.len)
                    else this.base;
      var ret: sungeun_string;
      ret.len = 1;
      ret.base = sbase.substring(i);
      ret.incRefCntNoAlias();
      if sremote then chpl_free_c_string(sbase);
      return ret;
    }

    // Returns a string containing the string sliced by the given
    // range, or an empty string if the range does not overlap.
    inline proc substring(r: range) {
      if this.isDefaultValue() || r.size<=0 then return new sungeun_string();
      const sremote = this.home.id != here.id;
      const sbase = if sremote
                    then remoteStringCopy(this.home.id, this.base, this.len)
                    else this.base;
      var ret: sungeun_string;
      ret.len = r.size;
      ret.base = sbase.substring(r);
      ret.incRefCntNoAlias();
      if sremote then chpl_free_c_string(sbase);
      return ret;
    }

    inline proc _string_contains(a: sungeun_string, b: sungeun_string)
      return _string_contains(a.base, b.base);
  
    // Returns the index of the first occurrence of a substring within a
    // string or 0 if the substring is not in the string.
    //
    // Note: substring on ranges should probably be rewritten expliclity
    // rather than relying on promotion.
    inline proc indexOf(s: sungeun_string):int
      return this.base.indexOf(s.base);
    inline proc indexOf(s: c_string):int
      return this.base.indexOf(s);

  // ref count functions
    inline proc isAlias return this.refCnt != this.aliasRefCnt;

    inline proc incRefCnt() {
      if !this.isAlias then this.refCnt.inc();
                      else this.aliasRefCnt.inc();
    }

    inline proc incRefCntNoAlias() {
      this.refCnt.inc();
    }

    inline proc decRefCnt() {
      if !this.isAlias then return this.refCnt.dec();
                      else return this.aliasRefCnt.dec();
    }

    inline proc decRefCntNoAlias() {
      return this.refCnt.dec();
    }

    inline proc freeRefCnt() {
      delete this.refCnt;
      if this.isAlias then delete this.aliasRefCnt;
    }

    inline proc isDefaultValue() {
      if this.isAlias then return false;
      else return this.refCnt.read()==0 && this.base==_defaultOf(c_string);
    }

    proc writeThis(f: Writer) {
      if this.home.id != here.id {
        const tcs = remoteStringCopy(this.home.id, this.base, this.len);
        f.write(tcs);
        chpl_free_c_string(tcs);
      } else {
        f.write(this.base);
      }
    }
  }

  /* We could also use _deafultOf(), but currently you can't define a
     _defaultOf() with a "ignore noinit" type.
  */
  /*
  inline proc _defaultOf(type t) where t: sungeun_string {
    var ret: sungeun_string; // this causes recursive inlining
    ...
    return ret;
  }
  */

  pragma "donor fn"
  pragma "auto copy fn"
    // We'd like this to be by ref, but doing so leads to an internal
    // compiler error.  See
    // $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
    /*inline*/ proc chpl__autoCopy(/*ref*/ s: sungeun_string) {
    if noisyConstructor then writeln("in autoCopy()");
    pragma "no auto destroy"
    var ret: sungeun_string;
    if !s.isDefaultValue() {
      ret.home = s.home;
      ret.base = s.base;
      ret.len = s.len;
      if !s.isAlias then ret.aliasRefCnt = s.refCnt;
                    else ret.aliasRefCnt = s.aliasRefCnt;
      ret.incRefCnt();
    }
    if noisyConstructor then writeln("leaving autoCopy()");
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
  inline proc chpl__initCopy(ref s: sungeun_string) {
    if noisyConstructor then writeln("in initCopy()");
    var ret: sungeun_string;
    if !s.isDefaultValue() {
      const slen = s.len; // cache the remote copy of len
      if s.home.id == here.id {
        if noisyConstructor then writeln("  local initCopy");
        if slen != 0 then ret.base = stringMove(_defaultOf(c_string), s.base, slen);
        else              ret.base = _defaultOf(c_string);
      } else {
        if noisyConstructor then writeln("  remote initCopy: ", s.home.id);
        if slen != 0 then ret.base = remoteStringCopy(s.home.id, s.base, slen);
        else              ret.base = _defaultOf(c_string);
      }
      ret.len = slen;
      ret.incRefCntNoAlias();
    }
    if noisyConstructor then writeln("leaving initCopy()");
    return ret;
  }

  //
  // Assignment functions
  //
  proc =(ref lhs: sungeun_string, rhs: sungeun_string) {
    if noisyConstructor then writeln("in proc =()");
    inline proc helpMe(ref lhs: sungeun_string, rhs: sungeun_string) {
      if rhs.home.id == here.id {
        if noisyConstructor then writeln("  rhs local");
        lhs.reinitString(rhs.base, rhs.len);
      } else {
        if noisyConstructor then writeln("  rhs remote: ", rhs.home.id);
        const rlen = rhs.len; // cache the remote copy of len
        // TODO: reuse the c_string from remoteStringCopy()
        const rs = remoteStringCopy(rhs.home.id, rhs.base, rlen);
        lhs.reinitString(rs, rlen);
        chpl_free_c_string(rs); // can better optimize this
      }
    }
    if lhs.home.id == here.id then {
      if noisyConstructor then writeln("  lhs local");
      helpMe(lhs, rhs);
    }
    else {
      if noisyConstructor then writeln("  lhs remote: ", lhs.home.id);
      on lhs do helpMe(lhs, rhs);
    }
    if noisyConstructor then writeln("leaving proc =()");
  }

  proc =(ref lhs: sungeun_string, rhs_c: c_string) {
    if noisyConstructor then writeln("in proc =() c_string");
    const hereId = here.id;
    // Make this some sort of local check once we have local types/vars
    if (rhs_c.locale.id != hereId) || (lhs.home.id != hereId) then
      halt("Cannot assign a remote c_string to a string.");
    const len = __primitive("string_length", rhs_c);
    lhs.reinitString(rhs_c, len);
    if noisyConstructor then writeln("leaving proc =() c_string");
  }

  //
  // Concatenation
  //
  proc +(s0: sungeun_string, s1: sungeun_string) {
    if noisyConstructor then writeln("in proc +()");
    var ret: sungeun_string;
    const s0len = s0.len;
    const s1len = s1.len;
    ret.len = s0len + s1len;
    const s0remote = s0.home.id != here.id;
    const s0base = if s0remote
                   then remoteStringCopy(s0.home.id, s0.base, s0len)
                   else s0.base;
    const s1remote = s1.home.id != here.id;
    const s1base = if s1remote
                   then remoteStringCopy(s1.home.id, s1.base, s1len)
                   else s1.base;
    ret.base = __primitive("string_concat", s0base, s1base);
    ret.incRefCntNoAlias();
    if s0remote then chpl_free_c_string(s0base);
    if s1remote then chpl_free_c_string(s1base);
    if noisyConstructor then writeln("leaving proc +()");
    return ret;
  }

  proc +(s: sungeun_string, cs: c_string) {
    if cs.locale.id != here.id then
      halt("Cannot concatenate a remote c_string.");
    if noisyConstructor then writeln("in proc +() string+c_string");
    var ret: sungeun_string;
    const slen = s.len;
    ret.len = slen + cs.length;
    const sremote = s.home.id != here.id;
    const sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, slen)
                  else s.base;
    ret.base = __primitive("string_concat", sbase, cs);
    ret.incRefCntNoAlias();
    if sremote then chpl_free_c_string(sbase);
    if noisyConstructor then writeln("leaving proc +() string+c_string");
    return ret;
  }

  proc +(cs: c_string, s: sungeun_string) {
    if cs.locale.id != here.id then
      halt("Cannot concatenate a remote c_string.");
    if noisyConstructor then writeln("in proc +() c_string+string");
    var ret: sungeun_string;
    const slen = s.len;
    ret.len = cs.length + slen;
    const sremote = s.home.id != here.id;
    const sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, slen)
                  else s.base;
    ret.base = __primitive("string_concat", cs, sbase);
    ret.incRefCntNoAlias();
    if sremote then chpl_free_c_string(sbase);
    if noisyConstructor then writeln("leaving proc +() c_string+string");
    return ret;
  }

  //
  // Concatenation with other types is done by casting to c_string
  //
  inline proc concatHelp(s: sungeun_string, x:?t) where t != sungeun_string {
    const cs = x:c_string;
    const ret = s + cs;
    if !_isBooleanType(x.type) && !_isEnumeratedType(x.type) then
      chpl_free_c_string(cs);
    return ret;
  }

  inline proc concatHelp(x:?t, s: sungeun_string) where t != sungeun_string  {
    const cs = x:c_string;
    const ret = cs + s;
    if !_isBooleanType(x.type) && !_isEnumeratedType(x.type) then
      chpl_free_c_string(cs);
    return ret;
  }

  inline proc +(s: sungeun_string, x: numeric) return concatHelp(s, x);
  inline proc +(x: numeric, s: sungeun_string) return concatHelp(x, s);
  inline proc +(s: sungeun_string, x: enumerated) return concatHelp(s, x);
  inline proc +(x: enumerated, s: sungeun_string) return concatHelp(x, s);
  inline proc +(s: sungeun_string, x: bool) return concatHelp(s, x);
  inline proc +(x: bool, s: sungeun_string) return concatHelp(x, s);


  //
  // Relational operators
  //
  // NOTE: Should we define these between string and c_string?
  inline proc ==(a: sungeun_string, b: sungeun_string)
    return (__primitive("string_compare", a.base, b.base) == 0);
  inline proc !=(a: sungeun_string, b: sungeun_string)
    return (__primitive("string_compare", a.base, b.base) != 0);

  inline proc <=(a: sungeun_string, b: sungeun_string) return a.base<=b.base;
  inline proc >=(a: sungeun_string, b: sungeun_string) return a.base>=b.base;
  inline proc <(a: sungeun_string, b: sungeun_string) return a.base<b.base;
  inline proc >(a: sungeun_string, b: sungeun_string) return a.base>b.base;

  //
  // Primitive string functions and methods
  //
  inline proc ascii(a: string) return ascii(a.base);

  //
  // Casts
  //

  // Cast from c_string to string
  // pragma "compiler generated"
  inline proc _cast(type t, cs: c_string) where t==sungeun_string {
    if noisyConstructor then writeln("in _cast() c_string-string");
    if cs.locale.id != here.id then
      halt("Cannot cast a remote c_string to string.");
    var ret: sungeun_string;
    ret.len = cs.length;
    if ret.len != 0 then ret.base = __primitive("string_copy", cs);
    ret.incRefCntNoAlias();
    if noisyConstructor then writeln("leaving _cast() c_string-string");
    return ret;
  }

  // Other casts to strings use c_string
  // pragma "compiler generated"
  inline proc _cast(type t, x) where t==sungeun_string && x.type != c_string {
    if noisyConstructor then writeln("in _cast() ", typeToString(t), "-string");
    const cs = x:c_string;
    var ret: sungeun_string;
    ret.len = cs.length;
    assert(ret.len != 0);
    if _isBooleanType(x.type) || _isEnumeratedType(x.type) then
      // These are returned as string literals so copy the string
      ret.base = stringMove(_defaultOf(c_string), cs, ret.len);
    else
      ret.base = cs;
    ret.incRefCntNoAlias();
    if noisyConstructor then writeln("leaving _cast() ", typeToString(t),
                                     "-string");
    return ret;
  }

  // Cast from string to c_string
  // This currently does not allocate a new c_string
  // It is exactly the same as string.c_str()
  // pragma "compiler generated"
  inline proc _cast(type t, s: sungeun_string) where t==c_string {
    if noisyConstructor then writeln("in _cast() string-c_string");
    if s.home.id != here.id then
      halt("Cannot cast a remote string to c_string");
    if noisyConstructor then writeln("leaving _cast() string-c_string");
    return s.base;
  }

  // Other casts from strings use c_string
  // pragma "compiler generated"
  inline proc _cast(type t, s: sungeun_string) where t!=c_string {
    if noisyConstructor then writeln("in _cast() string-", typeToString(t));
    const sremote = s.home.id != here.id;
    const sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, s.len)
                  else s.base;
    const ret = sbase:t;
    if sremote then chpl_free_c_string(sbase);
    if noisyConstructor then writeln("leaving _cast() string-",
                                     typeToString(t));
    return ret;
  }


  // No longer support toString()

}
