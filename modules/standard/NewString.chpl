/*
 * Copyright 2004-2014 Cray Inc.
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

// Uncommented when this becomes the default string implementation
// pragma "no use ChapelStandard"

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
 * - The base field is reference counted in the following manner.
 *   Every string has a ref count object and an alias ref count
 *   object.  If base is aliased, the the alias ref count object
 *   points to the ref count object of the base being aliased.
 *   Otherwise, the alias ref count object is equal to the ref count
 *   object.  There are set of ref counting support routines that
 *   update the "right" ref count, but can also bypassed if the alias
 *   check has already been performed.  I'm sure it's possible to
 *   implement this using a single ref count object, but this was
 *   easier to debug.
 *
 * TODOS:
 *
 * - When this becomes the default string implementation, the compiler
 *   may need to know of the string implementation as it does for
 *   array, etc.  Consider adding a pragma on the record.
 *
 * - Consider storing max size in the string record to enable more
 *   control of buffer reuse.
 * 
 * - Consider changing primitives like string_copy and string_concat
 *   to take optional (or maybe required) length arguments to avoid
 *   extra calls to strlen().
 *
 */

module BaseStringType {
  use CString;
  // This type must support the c_string interface
  // TODO: It should really be c_string_copy.
  type baseType = c_string;
  param baseTypeString = "c_string":c_string;
  // This used to be defined in terms of chpl_free_c_string_copy, but that one wants
  // the argument to be a c_string_copy.  Here, we lie about the interface to
  // chpl_rt_free_c_string(), so we can get around having to coerce the wrong
  // baseType (c_string) to the right one (c_string_copy).  The coercion will
  // allocate more memory, which is not what we want.  When baseType is
  // replaced by c_string_copy, this workaround can be removed.
  inline proc free_baseType(ref s) {
    pragma "insert line file info"
    extern proc chpl_rt_free_c_string(ref cs: c_string);
    pragma "insert line file info"
    extern proc chpl_rt_free_c_string_copy(ref csc: c_string_copy);

    if (s == _nullString) then return;

    type select s {
      when c_string do chpl_rt_free_c_string(s);
      when c_string_copy do chpl_rt_free_c_string_copy(s);
      otherwise halt("operand of free_baseType must be c_string or c_string_copy.");
    }
  }

  pragma "insert line file info"
  extern proc stringMove(dest: c_string, src: c_string, len: int): c_string;
  pragma "insert line file info"
  extern proc remoteStringCopy(src_loc: int, src_addr: c_string, len: int): c_string_copy;

  config param debugStrings = false;
}

// Chapel Strings
module NewString {
  use BaseStringType;

  class string_refcnt {
    var val: atomic_refcnt;
    inline proc inc(cnt=1) { val.inc(); }
    inline proc dec(cnt=1) return val.dec();
    inline proc read(cnt=1) return val._cnt.read();
  }

  // pragma "string record"
  pragma "ignore noinit"
  pragma "no default functions"
  record string_rec {
    var base: baseType;
    var len: int;
    var home: locale; // this does not change through assignment
    var refCnt: string_refcnt;
    var aliasRefCnt: string_refcnt;

    proc string_rec() {
      if debugStrings then writeln("in string()");
      this.home = here;
      this.refCnt = new string_refcnt();
      this.aliasRefCnt = this.refCnt;
      if debugStrings then writeln("leaving string()");
    }

    proc ref ~string_rec() {
      if debugStrings then writeln("in ~string()");
      if this.isEmptyString() {
        delete this.refCnt;
      } else {
        const refs =  decRefCnt();
        if refs < 0 then halt("string reference count is negative!");
        if refs == 0 then {
          if debugStrings then writeln("  freeing base: ", this.base);
          on this.home {
            free_baseType(this.base);
            freeRefCnt();
          }
        } else {
          if debugStrings then writeln("  not freeing base: ", this.base);
          if isAlias then delete this.refCnt;
        }
      }
      if debugStrings then writeln("leaving ~string()");
    }

    // This is assumed to be called from this.home
    proc ref reinitString(s: baseType, slen:int =-1) {
      assert(s.locale.id == here.id);
      assert(this.home.id == here.id);
      if debugStrings then writeln("in string.reinitString()");
      if this.isEmptyString() {
        if slen==0 || s==_defaultOf(baseType) then return; // nothing to do
        else this.incRefCntNoAlias(); // update my own ref count
      }
      const new_len = if slen == -1 then s.length else slen;
      // If the this.base is longer than s, then reuse the buffer
      if new_len != 0 {
        if new_len < this.len {
          // reuse the buffer
          this.base = stringMove(this.base, s, new_len);
        } else {
          // free the old buffer
          if this.len != 0 then
            on home do free_baseType(this.base);
          // allocate a new buffer
          this.base = stringMove(_defaultOf(baseType), s, new_len);
        }
      } else {
        // free the old buffer
        if this.len != 0 then free_baseType(this.base);
        this.base = _defaultOf(baseType); // empty string are always literals
      }
      this.len = new_len;
      if debugStrings then writeln("leaving string.reinitString()");
    }

    inline proc c_str() {
      if debugStrings then writeln("in .c_str()");
      if this.home.id != here.id then
        halt("Cannot call .c_str() on a remote string");
      if debugStrings then writeln("leaving .c_str()");
      return this.base;
    }

    inline proc length return len;
    inline proc size return len;

    // Returns a string containing the character at the given index of
    // the string, or an empty string if the index is out of bounds.
    inline proc substring(i: int) {
      var ret: string_rec;
      if !this.isEmptyString() && i>0 {
        const sremote = this.home.id != here.id;
        var   sbase = if sremote
                      then remoteStringCopy(this.home.id, this.base, this.len)
                      else this.base;
        const cs = sbase.substring(i);
        if cs != _defaultOf(baseType) {
          ret.base = cs;
          ret.len = 1;
          ret.incRefCntNoAlias();
        }
        if sremote then free_baseType(sbase);
      }
      return ret;
    }

    // Returns a string containing the string sliced by the given
    // range, or an empty string if the range does not overlap.
    inline proc substring(r: range) {
      var ret: string_rec;
      if !this.isEmptyString() && r.size>0 {
        const sremote = this.home.id != here.id;
        var   sbase = if sremote
                      then remoteStringCopy(this.home.id, this.base, this.len)
                      else this.base;
        const cs = sbase.substring(r);
        if cs != _defaultOf(baseType) {
          ret.base = cs;
          ret.len = cs.length;
          ret.incRefCntNoAlias();
        }
        if sremote then free_baseType(sbase);
      }
      return ret;
    }

    // Returns the index of the first occurrence of a substring within a
    // string or 0 if the substring is not in the string.
    inline proc find(s: string_rec):int {
      var ret: int;
      if this.isEmptyString() then return 0;
      if s.isEmptyString() then return 0;
      // Assume s.base is shorter than this.base, so go to the home locale
      on this.home {
        const sremote = s.home.id != here.id;
        var   sbase = if sremote
                      then remoteStringCopy(s.home.id, s.base, s.len)
                      else s.base;
        ret = this.base.indexOf(sbase);
        if sremote then free_baseType(sbase);
      }
      return ret;
    }

    inline proc find(s: baseType):int {
      if s.locale.id != here.id then
        halt("Cannot search a string with a remote "+baseTypeString);
      if this.isEmptyString() then return 0;
      if s==_defaultOf(baseType) then return 0;
      // We don't support remote c_string, so for this version, copy this.base
      const sremote = this.home.id != here.id;
      var   sbase = if sremote
                    then remoteStringCopy(this.home.id, this.base, this.len)
                    else this.base;
      const ret = sbase.indexOf(s);
      if sremote then free_baseType(sbase);
      return ret;
    }

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

    inline proc isEmptyString() {
      if this.isAlias then return false;
      else return this.len==0; // this should be enough of a check
    }

    proc writeThis(f: Writer) {
      if !this.isEmptyString() {
        if (this.home.id != here.id) {
          var tcs = remoteStringCopy(this.home.id, this.base, this.len);
          f.write(tcs);
          free_baseType(tcs);
        } else {
          f.write(this.base);
        }
      }
    }
  }

  pragma "donor fn"
  pragma "auto copy fn"
    // We'd like this to be by ref, but doing so leads to an internal
    // compiler error.  See
    // $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
    /*inline*/ proc chpl__autoCopy(/*ref*/ s: string_rec) {
    if debugStrings then writeln("in autoCopy()");
    pragma "no auto destroy"
    var ret: string_rec;
    if !s.isEmptyString() {
      ret.home = s.home;
      ret.base = s.base;
      ret.len = s.len;
      if !s.isAlias then ret.aliasRefCnt = s.refCnt;
                    else ret.aliasRefCnt = s.aliasRefCnt;
      ret.incRefCnt();
    }
    if debugStrings then writeln("leaving autoCopy()");
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
  inline proc chpl__initCopy(ref s: string_rec) {
    if debugStrings then writeln("in initCopy()");
    var ret: string_rec;
    if !s.isEmptyString() {
      const slen = s.len; // cache the remote copy of len
      if s.home.id == here.id {
        if debugStrings then writeln("  local initCopy");
        ret.base = stringMove(_defaultOf(baseType), s.base, slen);
      } else {
        if debugStrings then writeln("  remote initCopy: ", s.home.id);
        ret.base = remoteStringCopy(s.home.id, s.base, slen);
      }
      ret.len = slen;
      ret.incRefCntNoAlias();
    }
    if debugStrings then writeln("leaving initCopy()");
    return ret;
  }

  //
  // Assignment functions
  //
  proc =(ref lhs: string_rec, rhs: string_rec) {
    if debugStrings then writeln("in proc =()");
    inline proc helpMe(ref lhs: string_rec, rhs: string_rec) {
      if rhs.home.id == here.id {
        if debugStrings then writeln("  rhs local");
        lhs.reinitString(rhs.base, rhs.len);
      } else {
        if debugStrings then writeln("  rhs remote: ", rhs.home.id);
        const len = rhs.len; // cache the remote copy of len
        // TODO: reuse the base from remoteStringCopy()
        var rs = if len!=0 then remoteStringCopy(rhs.home.id, rhs.base, len)
                   else _defaultOf(baseType);
        lhs.reinitString(rs, len);
        if len!=0 then free_baseType(rs);
      }
    }
    if lhs.home.id == here.id then {
      if debugStrings then writeln("  lhs local");
      helpMe(lhs, rhs);
    }
    else {
      if debugStrings then writeln("  lhs remote: ", lhs.home.id);
      on lhs do helpMe(lhs, rhs);
    }
    if debugStrings then writeln("leaving proc =()");
  }

  proc =(ref lhs: string_rec, rhs_c: baseType) {
    if debugStrings then writeln("in proc =() "+baseTypeString);
    const hereId = here.id;
    // Make this some sort of local check once we have local types/vars
    if (rhs_c.locale.id != hereId) || (lhs.home.id != hereId) then
      halt("Cannot assign a remote "+baseTypeString+" to a string.");
    const len = rhs_c.length;
    lhs.reinitString(rhs_c, len);
    if debugStrings then writeln("leaving proc =() "+baseTypeString);
  }

  //
  // Concatenation
  //
  proc +(s0: string_rec, s1: string_rec) {
    if debugStrings then writeln("in proc +()");
    if s0.isEmptyString() then return s1;
    if s1.isEmptyString() then return s0;
    var ret: string_rec;
    const s0len = s0.len;
    const s1len = s1.len;
    ret.len = s0len + s1len;
    const s0remote = s0.home.id != here.id;
    var   s0base = if s0remote
                   then remoteStringCopy(s0.home.id, s0.base, s0len)
                   else s0.base;
    const s1remote = s1.home.id != here.id;
    var   s1base = if s1remote
                   then remoteStringCopy(s1.home.id, s1.base, s1len)
                   else s1.base;
    ret.base = s0base+s1base;
    ret.incRefCntNoAlias();
    if s0remote then free_baseType(s0base);
    if s1remote then free_baseType(s1base);
    if debugStrings then writeln("leaving proc +()");
    return ret;
  }

  proc +(s: string_rec, cs: baseType) {
    if cs.locale.id != here.id then
      halt("Cannot concatenate a remote "+baseTypeString+".");
    if debugStrings then writeln("in proc +() string+"+baseTypeString);
    if cs == _defaultOf(baseType) then return s;
    if s.isEmptyString() then return cs:string_rec;
    var ret: string_rec;
    const slen = s.len;
    ret.len = slen + cs.length;
    const sremote = s.home.id != here.id;
    var   sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, slen)
                  else s.base;
    ret.base = sbase+cs;
    ret.incRefCntNoAlias();
    if sremote then free_baseType(sbase);
    if debugStrings then writeln("leaving proc +() string+"+baseTypeString);
    return ret;
  }

  proc +(s: string_rec, ref cs: c_string_copy) {
    if cs.locale.id != here.id then
      halt("Cannot concatenate a remote "+baseTypeString+"_copy.");
    if debugStrings then writeln("in proc +() string+"+baseTypeString+"_copy");
    if cs == _defaultOf(c_string_copy) then return s;
    if s.isEmptyString() then return cs:string_rec;
    var ret: string_rec;
    const slen = s.len;
    ret.len = slen + cs.length;
    const sremote = s.home.id != here.id;
    var   sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, slen)
                  else s.base;
    ret.base = sbase+cs;
    ret.incRefCntNoAlias();
    if sremote then free_baseType(sbase);
    free_baseType(cs);
    if debugStrings then writeln("leaving proc +() string+"+baseTypeString+"_copy");
    return ret;
  }

  proc +(cs: baseType, s: string_rec) {
    if cs.locale.id != here.id then
      halt("Cannot concatenate a remote "+baseTypeString+".");
    if debugStrings then writeln("in proc +() "+baseTypeString+"+string");
    if cs == _defaultOf(baseType) then return s;
    if s.isEmptyString() then return cs:string_rec;
    var ret: string_rec;
    const slen = s.len;
    ret.len = cs.length + slen;
    const sremote = s.home.id != here.id;
    var   sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, slen)
                  else s.base;
    ret.base = cs+sbase;
    ret.incRefCntNoAlias();
    if sremote then free_baseType(sbase);
    if debugStrings then writeln("leaving proc +() "+baseTypeString+"+string");
    return ret;
  }

  proc +(ref cs: c_string_copy, s: string_rec) {
    if cs.locale.id != here.id then
      halt("Cannot concatenate a remote "+baseTypeString+"_copy.");
    if debugStrings then writeln("in proc +() "+baseTypeString+"_copy+string");
    if cs == _defaultOf(c_string_copy) then return s;
    if s.isEmptyString() then return cs:string_rec;
    var ret: string_rec;
    const slen = s.len;
    ret.len = cs.length + slen;
    const sremote = s.home.id != here.id;
    var   sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, slen)
                  else s.base;
    ret.base = cs+sbase;
    ret.incRefCntNoAlias();
    if sremote then free_baseType(sbase);
    free_baseType(cs);
    if debugStrings then writeln("leaving proc +() "+baseTypeString+"+string");
    return ret;
  }

  //
  // Concatenation with other types is done by casting to the baseType
  //
  inline proc concatHelp(s: string_rec, x:?t) where t != string_rec {
    var cs = x:c_string_copy;
    const ret = s + cs;
    return ret;
  }

  inline proc concatHelp(x:?t, s: string_rec) where t != string_rec  {
    var cs = x:c_string_copy;
    const ret = cs + s;
    return ret;
  }

  inline proc +(s: string_rec, x: numeric) return concatHelp(s, x);
  inline proc +(x: numeric, s: string_rec) return concatHelp(x, s);
  inline proc +(s: string_rec, x: enumerated) return concatHelp(s, x);
  inline proc +(x: enumerated, s: string_rec) return concatHelp(x, s);
  inline proc +(s: string_rec, x: bool) return concatHelp(s, x);
  inline proc +(x: bool, s: string_rec) return concatHelp(x, s);

  //
  // Relational operators
  //
  // Poor man's substitute for first class function support
  enum relType {eq=1, neq, lt, lte, gt, gte};
  inline proc relationalHelp(a: string_rec, b: string_rec,
                             param op: relType) {
    inline proc doOp(a: baseType, b: baseType, param op: relType) {
      select op {
        when relType.eq do return a==b;
        when relType.neq do return a!=b;
        when relType.lt do return a<b;
        when relType.lte do return a<=b;
        when relType.gt do return a>b;
        when relType.gte do return a>=b;
        otherwise compilerError("op not supported (", op:baseType, ")");
      }
    }
    var ret: bool;
    if a.home.id == b.home.id {
      on a.home do ret = doOp(a.base, b.base, op);
    } else {
      // Copy the a if it is remote and not the empty string
      var abase: baseType;
      const alen = a.len;
      const aIsEmpty = a.isEmptyString();
      const aremote = (a.home.id!=here.id) && !aIsEmpty;
      if aremote then abase = remoteStringCopy(a.home.id, a.base, alen);
      else if !aIsEmpty then abase = a.base;
      else abase = _defaultOf(baseType);
      // Ditto for b
      var bbase: baseType;
      const blen = b.len;
      const bIsEmpty = b.isEmptyString();
      const bremote = (b.home.id!=here.id) && !bIsEmpty;
      if bremote then bbase = remoteStringCopy(b.home.id, b.base, blen);
      else if !bIsEmpty then bbase = b.base;
      else bbase = _defaultOf(baseType);

      ret = doOp(abase, bbase, op);

      if aremote then free_baseType(abase);
      if bremote then free_baseType(bbase);
    }
    return ret;
  }

  inline proc ==(a: string_rec, b: string_rec)
    return relationalHelp(a, b, relType.eq);
  inline proc !=(a: string_rec, b: string_rec)
    return relationalHelp(a, b, relType.neq);
  inline proc <(a: string_rec, b: string_rec)
    return relationalHelp(a, b, relType.lt);
  inline proc <=(a: string_rec, b: string_rec)
    return relationalHelp(a, b, relType.lte);
  inline proc >(a: string_rec, b: string_rec)
    return relationalHelp(a, b, relType.gt);
  inline proc >=(a: string_rec, b: string_rec)
    return relationalHelp(a, b, relType.gte);

  //
  // Primitive string functions and methods
  //
  inline proc ascii(s: string_rec) {
    var ret: int(32); // return type of ascii primitive
    on s.home do ret = ascii(s.base);
    return ret;
  }

  //
  // Casts
  //

  // Cast from baseType to string
  inline proc _cast(type t, cs: baseType) where t==string_rec {
    if debugStrings then writeln("in _cast() "+baseTypeString+"-string");
    if cs.locale.id != here.id then
      halt("Cannot cast a remote "+baseTypeString+" to string.");
    var ret: string_rec;
    ret.len = cs.length;
    if ret.len != 0 then ret.base = __primitive("string_copy", cs);
    ret.incRefCntNoAlias();
    if debugStrings then writeln("leaving _cast() "+baseTypeString+"-string");
    return ret;
  }

  // Other casts to strings use the baseType
  inline proc _cast(type t, x) where t==string_rec && x.type != baseType {
    if debugStrings then writeln("in _cast() ", typeToString(t), "-string");
    const cs = x:c_string_copy;
    var ret: string_rec;
    ret.len = cs.length;
    assert(ret.len != 0);
    ret.base = cs;
    ret.incRefCntNoAlias();
    if debugStrings then writeln("leaving _cast() ", typeToString(t),
                                 "-string");
    return ret;
  }

  // Cast from string to the baseType
  // This currently does not allocate a new baseType string
  // It is exactly the same as string.c_str()
  inline proc _cast(type t, s: string_rec) where t==baseType {
    if debugStrings then writeln("in _cast() string-c_"+baseTypeString);
    if s.home.id != here.id then
      halt("Cannot cast a remote string to "+baseTypeString);
    if debugStrings then writeln("leaving _cast() string-"+baseTypeString);
    return s.base;
  }

  // Other casts from strings use the baseType
  inline proc _cast(type t, s: string_rec) where t!=baseType {
    if debugStrings then writeln("in _cast() string-", typeToString(t));
    const sremote = s.home.id != here.id;
    var   sbase = if sremote
                  then remoteStringCopy(s.home.id, s.base, s.len)
                  else s.base;
    // Note that any error checking on the string is done in the runtime
    const ret = sbase:t;
    if sremote then free_baseType(sbase);
    if debugStrings then writeln("leaving _cast() string-", typeToString(t));
    return ret;
  }


  // Functions that are no longer supported:
  // - toString()
  // - _string_contains()

}
