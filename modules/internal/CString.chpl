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
  inline proc _defaultOf(type t) param where t: c_string return c"";
  inline proc _defaultOf(type t) where t == c_string_copy return _nullString;

  //inline proc c_string.c_str() return this;

  // These routines consume the c_string_copy argument.  In terms of MM, this
  // is the same as if the caller had called chpl_free_string_copy()
  extern proc string_from_c_string_copy(ref cstrc:c_string_copy,
                                        hasLen:bool, len:int) : string;
  // TODO
  //inline proc toString(cstrc:c_string_copy) : string {
  //  return string_from_c_string_copy(cstrc, false, 0);
  //}
  //inline proc toString(cstrc:c_string_copy, len:int) : string {
  //  return string_from_c_string_copy(cstrc, true, len);
  //}

  // We can't use the catch-all initCopy or autoCopy because of the
  // transformation of c_strings into string for generic parameters
  // TODO: cant specify return type or else we run into a bug were we do an
  //       assignment out of these fuctions, causing acess to uninitalized
  //       memory for c_string_copy
  pragma "init copy fn"
  inline proc chpl__initCopy(x: c_string) : c_string {
    return x;
  }
  pragma "init copy fn"
  inline proc chpl__initCopy(x: c_string_copy) : c_string_copy {
    return x;
  }

  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: c_string) : c_string {
    return x;
  }
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: c_string_copy) : c_string_copy {
    return x;
  }

  //proc typeToString(type t) param {
  //  return __primitive("typeToString", t);
  //}

  //proc typeToString(x) param {
  //  compilerError("typeToString()'s argument must be a type, not a value");
  //}

  inline proc ==(param s0: c_string, param s1: c_string) param {
    return __primitive("string_compare", s0, s1) == 0;
  }

  inline proc ==(s0: c_string, s1: c_string) {
    return __primitive("string_compare", s0, s1) == 0;
  }

//  inline proc ==(s0: string, s1: c_string) {
//    return __primitive("string_compare", s0.c_str(), s1) == 0;
//  }
//
//  inline proc ==(s0: c_string, s1: string) {
//    return __primitive("string_compare", s0, s1.c_str()) == 0;
//  }

  inline proc !=(param s0: c_string, param s1: c_string) param {
    return __primitive("string_compare", s0, s1) != 0;
  }

  inline proc !=(s0: c_string, s1: c_string) {
    return __primitive("string_compare", s0, s1) != 0;
  }

//  inline proc !=(s0: string, s1: c_string) {
//    return __primitive("string_compare", s0.c_str(), s1) != 0;
//  }
//
//  inline proc !=(s0: c_string, s1: string) {
//    return __primitive("string_compare", s0, s1.c_str()) != 0;
//  }

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

  // A c_string_copy can always be used as a c_string.
  inline proc _cast(type t, x: c_string_copy) where t == c_string {
    return __primitive("cast", t, x);
  }

  extern proc chpl_bool_to_c_string(x:bool) : c_string;
  inline proc _cast(type t, x: bool(?w)) where t == c_string {
    return chpl_bool_to_c_string(x:bool);
  }
  inline proc _cast(type t, x: bool(?w)) where t == c_string_copy {
    return __primitive("string_copy", chpl_bool_to_c_string(x:bool));
  }

  inline proc _cast(type t, x:enumerated) where t == c_string_copy {
    // Use the compiler-generated enum to c_string conversion.
    var cs = _cast(c_string, x);
    return __primitive("string_copy", cs);
  }

  inline proc _cast(type t, x:integral) where t == c_string_copy {
    extern proc integral_to_c_string_copy(x:int(64), size:uint(32), isSigned: bool) : c_string_copy ;
    return integral_to_c_string_copy(x:int(64), numBytes(x.type), isIntType(x.type));
  }

  //
  // casts from c_string to bool types
  //
  inline proc _cast(type t, x:c_string) where isBoolType(t)
  {
    pragma "insert line file info"
    extern proc c_string_to_chpl_bool(x:c_string) : bool;
    return c_string_to_chpl_bool(x) : t;
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

  inline proc _cast(type t, x:c_string) where t == int(8)
    return c_string_to_int8_t(x);
  inline proc _cast(type t, x:c_string) where t == int(16)
    return c_string_to_int16_t(x);
  inline proc _cast(type t, x:c_string) where t == int(32)
    return c_string_to_int32_t(x);
  inline proc _cast(type t, x:c_string) where t == int(64)
    return c_string_to_int64_t(x);
  inline proc _cast(type t, x:c_string) where t == uint(8)
    return c_string_to_uint8_t(x);
  inline proc _cast(type t, x:c_string) where t == uint(16)
    return c_string_to_uint16_t(x);
  inline proc _cast(type t, x:c_string) where t == uint(32)
    return c_string_to_uint32_t(x);
  inline proc _cast(type t, x:c_string) where t == uint(64)
    return c_string_to_uint64_t(x);

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

  inline proc _cast(type t, x:c_string) where t == real(32)
    return c_string_to_real32(x);
  inline proc _cast(type t, x:c_string) where t == real(64)
    return c_string_to_real64(x);
  inline proc _cast(type t, x:c_string) where t == imag(32)
    return c_string_to_imag32(x);
  inline proc _cast(type t, x:c_string) where t == imag(64)
    return c_string_to_imag64(x);

  //
  // casts from c_string to complex types
  //
  pragma "insert line file info"
  extern proc c_string_to_complex64(x:c_string) : complex(64);
  pragma "insert line file info"
  extern proc c_string_to_complex128(x:c_string) : complex(128);

  inline proc _cast(type t, x:c_string) where t == complex(64)
    return c_string_to_complex64(x);
  inline proc _cast(type t, x:c_string) where t == complex(128)
    return c_string_to_complex128(x);

  //
  // casts from complex
  //
  inline proc _cast(type t, x: complex(?w)) where t == c_string_copy {
    if isnan(x.re) || isnan(x.im) then
      return __primitive("string_copy", "nan");
    var re = (x.re):c_string_copy;
    var im: c_string_copy;
    var op: c_string;
    if x.im < 0 {
      im = (-x.im):c_string_copy;
      op = " - ";
    } else if im == "-0.0" {
      im = "0.0":c_string_copy;
      op = " - ";
    } else {
      im = (x.im):c_string_copy;
      op = " + ";
    }
    // TODO: Add versions of the concatenation operator that consume their
    // c_string_copy arg or args.
    const ts0 = re + op;
    chpl_free_c_string(re);
    const ts1 = ts0 + im;
    chpl_free_c_string(ts0);
    chpl_free_c_string(im);
    const ret = ts1 + "i";
    chpl_free_c_string(ts1);
    return ret;
  }
  // TODO: This is only in place to support test code in types/string/sungeun.
  // Nor user nor module code should use this cast, because it strips ownership
  // from the c_string_copy returned by the above cast without first arranging
  // for its disposal.
  inline proc _cast(type t, x:complex(?w)) where t == c_string
    return _cast(c_string_copy, x);

  extern proc real_to_c_string_copy(x:real(64), isImag: bool) : c_string_copy ;
  //
  // casts from real
  //
  inline proc _cast(type t, x:real(?w)) where t == c_string_copy {
    return real_to_c_string_copy(x:real(64), false);
  }
  // TODO: This is only in place to support test code in types/string/sungeun.
  // Nor user nor module code should use this cast, because it strips ownership
  // from the c_string_copy returned by the above cast without first arranging
  // for its disposal.
  inline proc _cast(type t, x:real(?w)) where t == c_string
    return _cast(c_string_copy, x);

  //
  // casts from imag
  //
  inline proc _cast(type t, x:imag(?w)) where t == c_string_copy {
    // The Chapel version of the imag --> real cast smashes it flat rather than
    // just stripping off the "i".  See ChapelBase:965.
    var r = __primitive("cast", real(64), x);
    return real_to_c_string_copy(r, true);
  }
  // TODO: This is only in place to support test code in types/string/sungeun.
  // Nor user nor module code should use this cast, because it strips ownership
  // from the c_string_copy returned by the above cast without first arranging
  // for its disposal.
  inline proc _cast(type t, x:imag(?w)) where t == c_string
    return _cast(c_string_copy, x);

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

  // Looks like we still need c_str + c_str unless I want to change even more
  // module code code. TODO: Change the module code.
  inline proc +(a: c_string, b: c_string) {
    return __primitive("string_concat", a, b);
  }
  /*
  inline proc +(a:c_string, b:c_string_copy) {
    return __primitive("string_concat", a, b);
  }
  inline proc +(a:c_string_copy, b:c_string_copy) {
    return __primitive("string_concat", a, b);
  }
  inline proc +(a:c_string_copy, b:c_string) {
    return __primitive("string_concat", a, b);
  }
  */

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

