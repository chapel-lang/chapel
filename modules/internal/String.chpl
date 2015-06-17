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

// Chapel Strings
module String {
  use CString;

  pragma "default string value" extern var defaultStringValue: string = noinit;

  // The following method is called by the compiler to determine the default
  // value of a given type.  For strings, this should return the value
  // defined above.
  // strings will be records, so param is not possible for them
  inline proc _defaultOf(type t) where t == string return defaultStringValue;

  // String concatenation
  inline proc +(s: string, x: string) {
    var cs = __primitive("string_concat", s.c_str(), x.c_str());
    const ret = toString(cs);
    // toString steals the c_string_copy, so no need to free it here.
    return ret;
  }

  inline proc +(s: c_string, x: string) {
    var cs = __primitive("string_concat", s, x.c_str());
    const ret = toString(cs);
    // toString steals the c_string_copy, so no need to free it here.
    return ret;
  }

  inline proc +(s: string, x: c_string) {
    var cs = __primitive("string_concat", s.c_str(), x);
    const ret = toString(cs);
    // toString steals the c_string_copy, so no need to free it here.
    return ret;
  }

  inline proc +(s: string, x: numeric)
    return s + x:string;

  inline proc +(x: numeric, s: string)
    return x:string + s;

  inline proc +(s: string, x: enumerated)
    return s + x:string;

  inline proc +(x: enumerated, s: string)
    return x:string + s;

  inline proc +(s: string, x: bool)
    return s + x:string;

  inline proc +(x: bool, s: string)
    return x:string + s;

  proc typeToString(type t) param {
    return __primitive("typeToString", t);
  }
  
  proc typeToString(x) param {
    compilerError("typeToString()'s argument must be a type, not a value");
  }

  // This implies that the *representation* of strings is shared.
  // If strings are reimplemented as classes or records, a less trivial
  // implementation for assignment will become necessary.
  inline proc =(ref a: string, b: string) { __primitive("move", a, b); }

  inline proc ==(a: string, b: string) return (__primitive("string_compare", a.c_str(), b.c_str()) == 0);
  inline proc !=(a: string, b: string) return (__primitive("string_compare", a.c_str(), b.c_str()) != 0);


  inline proc <=(a: string, b: string) return a.c_str()<=b.c_str();
  inline proc >=(a: string, b: string) return a.c_str()>=b.c_str();
  inline proc <(a: string, b: string) return a.c_str()<b.c_str();
  inline proc >(a: string, b: string) return a.c_str()>b.c_str();

  //
  // primitive string functions and methods
  //
  inline proc ascii(a: string) return ascii(a.c_str());
  inline proc string.length return this.c_str().length;
  inline proc string.size return this.length;
  inline proc string.substring(i: int) {
    var cs = this.c_str().substring(i);
    const ret = toString(cs);
    // toString steals the returned c_string_copy, so no need to free it.
    return ret;
  }
  inline proc string.substring(r: range(?)) {
    var cs = this.c_str().substring(r);
    const ret = toString(cs);
    // toString steals the returned c_string_copy, so no need to free it.
    return ret;
  }
  
  inline proc _string_contains(a: string, b: string)
    return _string_contains(a.c_str(), b.c_str());

  /* args: any number of strings
     return: Returns true if this starts with one of the strings specified in args
   */
  inline proc string.startsWith(args ...?k):bool {
    for param i in 1..k {
      if (this.substring(0..args(i).length) == args(i))
        then return true;
    }
    return false;
  }

  
  /* Returns the index of the first occurrence of a substring within a string,
      or 0 if the substring is not in the string.
   */
  inline proc string.indexOf(substring:string):int
    return this.c_str().indexOf(substring.c_str());
  inline proc string.indexOf(substring:c_string):int
    return this.c_str().indexOf(substring);
  
  // cast to and from Chapel strings use c_string
  pragma "compiler generated"
  inline proc c_string.cast(type t) where t == string {
    var cs = this.cast(c_string_copy);
    // Note, this uses a non-allocating toString(), and steals cs (no need to free).
    const ret = toString(cs);
    return ret;
  }

  // The "compiler generated" flag is used, because this cast has to be weaker
  // than the c_string-to-enum cast generated in buildDefaultFunctions().
  // The circularity is caused by the built-in c_string-to-string coercion.
  // TODO: If string literals were all actually chpl_strings, then I think the
  // coercion (and this cruft) could be removed.
  pragma "compiler generated"
  inline proc string.cast(type t) where isEnumType(t)
    return this.c_str().cast(t);

  inline proc string.cast(type t) where isBoolType(t) || isNumericType(t)
    return this.c_str().cast(t);

  //
  // casts from c_string to bool types
  //
  inline proc c_string.cast(type t) where isBoolType(t)
  {
    pragma "insert line file info"
    extern proc c_string_to_chpl_bool(x:c_string) : bool;
    return c_string_to_chpl_bool(this) : t;
  }

  //
  // casts from c_string to integer types
  //
  pragma "insert line file info"
  extern proc c_string_to_int8_t  (x:c_string) : int(8); 
  pragma "insert line file info"
  extern proc c_string_to_int16_t (x:c_string) : int(16);
  pragma "insert line file info"
  extern proc c_string_to_int32_t (x:c_string) : int(32);
  pragma "insert line file info"
  extern proc c_string_to_int64_t (x:c_string) : int(64);
  pragma "insert line file info"
  extern proc c_string_to_uint8_t (x:c_string) : uint(8); 
  pragma "insert line file info"
  extern proc c_string_to_uint16_t(x:c_string) : uint(16);
  pragma "insert line file info"
  extern proc c_string_to_uint32_t(x:c_string) : uint(32);
  pragma "insert line file info"
  extern proc c_string_to_uint64_t(x:c_string) : uint(64);

  inline proc c_string.cast(type t) where t == int(8)
    return c_string_to_int8_t(this);
  inline proc c_string.cast(type t) where t == int(16)
    return c_string_to_int16_t(this);
  inline proc c_string.cast(type t) where t == int(32)
    return c_string_to_int32_t(this);
  inline proc c_string.cast(type t) where t == int(64)
    return c_string_to_int64_t(this);
  inline proc c_string.cast(type t) where t == uint(8)
    return c_string_to_uint8_t(this);
  inline proc c_string.cast(type t) where t == uint(16)
    return c_string_to_uint16_t(this);
  inline proc c_string.cast(type t) where t == uint(32)
    return c_string_to_uint32_t(this);
  inline proc c_string.cast(type t) where t == uint(64)
    return c_string_to_uint64_t(this);

  //
  // casts from c_string to real/imag types
  //
  pragma "insert line file info"
  extern proc c_string_to_real32(x:c_string) : real(32);
  pragma "insert line file info"
  extern proc c_string_to_real64(x:c_string) : real(64);
  pragma "insert line file info"
  extern proc c_string_to_imag32(x:c_string) : imag(32);
  pragma "insert line file info"
  extern proc c_string_to_imag64(x:c_string) : imag(64);
  
  inline proc c_string.cast(type t) where t == real(32)
    return c_string_to_real32(this);
  inline proc c_string.cast(type t) where t == real(64)
    return c_string_to_real64(this);
  inline proc c_string.cast(type t) where t == imag(32)
    return c_string_to_imag32(this);
  inline proc c_string.cast(type t) where t == imag(64)
    return c_string_to_imag64(this);

  //
  // casts from c_string to complex types
  //
  pragma "insert line file info"
  extern proc c_string_to_complex64(x:c_string) : complex(64);
  pragma "insert line file info"
  extern proc c_string_to_complex128(x:c_string) : complex(128);

  inline proc c_string.cast(type t) where t == complex(64)
    return c_string_to_complex64(this);
  inline proc c_string.cast(type t) where t == complex(128)
    return c_string_to_complex128(this);

  //
  // casts from complex
  //
  inline proc numeric.cast(type t)
    where isComplexType(this.type) && t == c_string_copy {
    if isnan(this.re) || isnan(this.im) then
      return __primitive("string_copy", "nan");
    var re = (this.re):c_string_copy;
    var im: c_string_copy;
    var op: c_string;
    if this.im < 0 {
      im = (-this.im):c_string_copy;
      op = " - ";
    } else if im == "-0.0" {
      im = "0.0":c_string_copy;
      op = " - ";
    } else {
      im = (this.im):c_string_copy;
      op = " + ";
    }
    // TODO: Add versions of the concatenation operator that consume their
    // c_string_copy arg or args.
    var ts0 = re + op;
    chpl_free_c_string_copy(re);
    var ts1 = ts0 + im;
    chpl_free_c_string_copy(ts0);
    chpl_free_c_string_copy(im);
    const ret = ts1 + "i";
    chpl_free_c_string_copy(ts1);
    return ret;
  }
  // TODO: This is only in place to support test code in types/string/sungeun.
  // Nor user nor module code should use this cast, because it strips ownership
  // from the c_string_copy returned by the above cast without first arranging
  // for its disposal.
  inline proc numeric.cast(type t)
    where isComplexType(this.type) && t == c_string
    return this.cast(c_string_copy);

  
  pragma "init copy fn"
  inline proc chpl__initCopy(a)
    where a.type == c_string || a.type == c_string_copy {
    // Currently, string representations are shared.
    // (See note on proc =(a:string, b:string) above.)
      return a;
  }
  
  proc chpldev_refToString(ref arg) {
  
    //
    // print out the address of class references as well
    //
    proc chpldev_classToString(x: object)
      return " (class = " + __primitive("ref to string", x) + ")";
    proc chpldev_classToString(x) return "";
  
    return __primitive("ref to string", arg) + chpldev_classToString(arg);
  }
  
}

// C strings
// extern type c_string; is a built-in primitive type
//
// In terms of how they are used, c_strings are a "close to the metal"
// representation, being in essence the common NUL-terminated C string.
//
// C string copies
// extern type c_string_copy is also a built-in primitive type.
// It is the same as a c_string, but in its case represents "owned" data.
// Low-level routines that allocate string data off the heap and return a deep
// copy (including string_copy, string_concat, string_index and string_select)
// have the return type of c_string_copy to denote that ownership.
//
// The difference is ignored by the C compiler, but Chapel treats them as
// different types.  This difference allows us to have two versions of
// toString: one makes a copy of its c_string argument, the other simply
// pointer-copies its c_string_copy argument.  Both effectively return an
// "owned" C string, which is how the internal chpl_string type is currently
// interpreted.  (The new record-based string implementation has different
// rules, but still makes use of the distinction between unowned c_strings and
// owned c_string_copies.
module CString {

  // The following method is called by the compiler to determine the default
  // value of a given type.
  inline proc _defaultOf(type t) param where t == c_string return "":c_string;
  inline proc _defaultOf(type t) where t == c_string_copy return _nullString;

  inline proc toString(cstr:c_string):string {
    return __primitive("string_from_c_string", cstr, 0, 0);
  }
  inline proc toString(cstr:c_string, len:int):string {
    return __primitive("string_from_c_string", cstr, 1, len);
  }

  // These routines consume the c_string_copy argument.  In terms of MM, this
  // is the same as if the caller had called chpl_free_string_copy()
  extern proc string_from_c_string_copy(ref cstrc:c_string_copy,
                                        hasLen:bool, len:int) : string;
  inline proc toString(ref cstrc:c_string_copy) : string {
    return string_from_c_string_copy(cstrc, false, 0);
  }
  inline proc toString(ref cstrc:c_string_copy, len:int) : string {
    return string_from_c_string_copy(cstrc, true, len);
  }

  // WARNING: The bytes pointed to by the c_string return value are still owned
  // by the "this" operand.  The returned c_string should not be freed!
  inline proc string.c_str():c_string {
    return __primitive("c_string_from_string", this);
  }

  inline proc ==(param s0: c_string, param s1: c_string) param {
    return __primitive("string_compare", s0, s1) == 0;
  }

  inline proc ==(s0: c_string, s1: c_string) {
    return __primitive("string_compare", s0, s1) == 0;
  }

  inline proc ==(s0: string, s1: c_string) {
    return __primitive("string_compare", s0.c_str(), s1) == 0;
  }

  inline proc ==(s0: c_string, s1: string) {
    return __primitive("string_compare", s0, s1.c_str()) == 0;
  }

  inline proc !=(param s0: c_string, param s1: c_string) param {
    return __primitive("string_compare", s0, s1) != 0;
  }

  inline proc !=(s0: c_string, s1: c_string) {
    return __primitive("string_compare", s0, s1) != 0;
  }

  inline proc !=(s0: string, s1: c_string) {
    return __primitive("string_compare", s0.c_str(), s1) != 0;
  }

  inline proc !=(s0: c_string, s1: string) {
    return __primitive("string_compare", s0, s1.c_str()) != 0;
  }

  inline proc <=(a: c_string, b: c_string) {
    return (__primitive("string_compare", a, b) <= 0);
  }

  inline proc <=(param a: c_string, param b: c_string) param {
    return (__primitive("string_compare", a, b) <= 0);
  }

  inline proc >=(a: c_string, b: c_string) {
    return (__primitive("string_compare", a, b) >= 0);
  }

  inline proc >=(param a: c_string, param b: c_string) param {
    return (__primitive("string_compare", a, b) >= 0);
  }

  inline proc <(a: c_string, b: c_string) {
    return (__primitive("string_compare", a, b) < 0);
  }

  inline proc <(param a: c_string, param b: c_string) param {
    return (__primitive("string_compare", a, b) < 0);
  }

  inline proc >(a: c_string, b: c_string) {
    return (__primitive("string_compare", a, b) > 0);
  }

  inline proc >(param a: c_string, param b: c_string) param {
    return (__primitive("string_compare", a, b) > 0);
  }

  inline proc =(ref a: c_string, b: c_string) {
    __primitive("=", a, b);
  }

  inline proc =(ref a: c_string, b: string) {
    __primitive("=", a, b.c_str());
  }

  inline proc =(ref a: string, b: c_string) {
    __primitive("=", a, toString(b));
  }

  // Create a fresh copy of the RHS string, first releasing the LHS.
  inline proc =(ref a: c_string_copy, b: c_string) {
    chpl_free_c_string_copy(a);
    var c = __primitive("string_copy", b);
    __primitive("=", a, c);
  }
  // Assume ownership of data brought by the RHS, first releasing the LHS.
  inline proc =(ref a: c_string_copy, b: c_string_copy) {
    chpl_free_c_string_copy(a);
    __primitive("=", a, b);
  }

  inline proc c_string.cast(type t) where t == string {
    return toString(this);
  }

  inline proc ref c_string_copy.cast(type t) where t == string {
    return toString(this);
  }

  inline proc string.cast(type t) where t == c_string {
    return this.c_str();
  }

  inline proc string.cast(type t) where t == c_string_copy {
    return __primitive("string_copy", this);
  }

  // A c_string_copy can always be used as a c_string.
  inline proc c_string_copy.cast(type t) where t == c_string {
    return __primitive("cast", t, this);
  }

  extern proc chpl_bool_to_c_string(x:bool) : c_string;
  inline proc boolean.cast(type t) where t == c_string {
    return chpl_bool_to_c_string(this:bool);
  }
  inline proc boolean.cast(type t) where t == c_string_copy {
    return __primitive("string_copy", chpl_bool_to_c_string(this:bool));
  }

  inline proc enumerated.cast(type t) where t == c_string_copy {
    // Use the compiler-generated enum to c_string conversion.
    var cs = this.cast(c_string);
    return __primitive("string_copy", cs);
  }

  inline proc integral.cast(type t) where t == c_string_copy {
    extern proc integral_to_c_string_copy(x:int(64), size:uint(32), isSigned: bool) : c_string_copy ;
    return integral_to_c_string_copy(this:int(64), numBytes(this.type),
        isIntType(this.type));
  }

  extern proc real_to_c_string_copy(x:real(64), isImag: bool) : c_string_copy ;
  //
  // casts from real
  //
  inline proc numeric.cast(type t)
    where isRealType(this.type) && t == c_string_copy {
    return real_to_c_string_copy(this:real(64), false);
  }
  // TODO: This is only in place to support test code in types/string/sungeun.
  // Nor user nor module code should use this cast, because it strips ownership
  // from the c_string_copy returned by the above cast without first arranging
  // for its disposal.
  inline proc numeric.cast(type t, x:real(?w))
    where isRealType(this.type) && t == c_string
    return this.cast(c_string_copy);

  //
  // casts from imag
  //
  inline proc numeric.cast(type t)
    where isImagType(this.type) && t == c_string_copy {
    // The Chapel version of the imag --> real cast smashes it flat rather than
    // just stripping off the "i".  See ChapelBase:965.
    var r = __primitive("cast", real(64), this);
    return real_to_c_string_copy(r, true);
  }
  // TODO: This is only in place to support test code in types/string/sungeun.
  // Nor user nor module code should use this cast, because it strips ownership
  // from the c_string_copy returned by the above cast without first arranging
  // for its disposal.
  inline proc numeric.cast(type t)
    where isImagType(this.type) && t == c_string
    return this.cast(c_string_copy);

  // Only support param c_string concatenation (for now)
  inline proc +(param a: c_string, param b: c_string) param
    return __primitive("string_concat", a, b);

  inline proc +(param s: c_string, param x: integral) param
    return __primitive("string_concat", s, x:c_string);

  inline proc +(param x: integral, param s: c_string) param
    return __primitive("string_concat", x:c_string, s);

  inline proc +(param s: c_string, param x: enumerated) param
    return __primitive("string_concat", s, x:c_string);

  inline proc +(param x: enumerated, param s: c_string) param
    return __primitive("string_concat", x:c_string, s);

  inline proc +(param s: c_string, param x: bool) param
    return __primitive("string_concat", s, x:c_string);

  inline proc +(param x: bool, param s: c_string) param
    return __primitive("string_concat", x:c_string, s);

  inline proc +(a: c_string, b: c_string)
    return __primitive("string_concat", a, b);

  proc c_string.writeThis(x: Writer) {
    x.write(this);
  }
  // The c_string_copy version is required, since apparently coercions are not
  // applied to "this".
  proc c_string_copy.writeThis(x: Writer) {
    x.write(this:c_string);
  }


  //
  // primitive c_string functions and methods
  //
  inline proc ascii(param a: c_string) param return __primitive("ascii", a);
  inline proc ascii(a: c_string) return __primitive("ascii", a);
  inline proc c_string.length return __primitive("string_length", this);
  inline proc c_string.size return this.length;
  inline proc c_string.substring(i: int)
    return __primitive("string_index", this, i);
  inline proc c_string.substring(r: range(?)) {
    var r2 = r[1..this.length];  // This may warn about ambiguously aligned ranges.
    var lo:int = r2.alignedLow, hi:int = r2.alignedHigh;
    return __primitive("string_select", this, lo, hi, r2.stride);
  }

  inline proc _string_contains(a: string, b: string)
    return __primitive("string_contains", a, b);

  inline proc param c_string.length param
    return __primitive("string_length", this);
  inline proc _string_contains(param a: c_string, param b: c_string) param
    return __primitive("string_contains", a, b);

  /* Returns the index of the first occurrence of a substring within a string,
     or 0 if the substring is not in the string.
  */
  inline proc c_string.indexOf(substring:c_string):int
    return string_index_of(this, substring);
  extern proc string_index_of(haystack:c_string, needle:c_string):int;

  // Use with care.  Not for the weak.
  inline proc chpl_free_c_string_copy(ref cs: c_string_copy) {
    pragma "insert line file info"
    extern proc chpl_rt_free_c_string(ref cs: c_string_copy);
    if (cs != _nullString) then chpl_rt_free_c_string(cs);
    // cs = _nullString;
  }

}

