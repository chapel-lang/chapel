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

// C strings
// extern type c_string; is a built-in primitive type
//
// In terms of how they are used, c_strings are a "close to the metal"
// representation, being in essence the common NUL-terminated C string.
module CString {
  use ChapelStandard;

  // The following method is called by the compiler to determine the default
  // value of a given type.
  inline proc _defaultOf(type t) where t: c_string return c_nil:c_string;

  //inline proc c_string.c_str() return this;

  pragma "init copy fn"
  inline proc chpl__initCopy(x: c_string) : c_string {
    return x;
  }

  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: c_string) : c_string {
    return x;
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

  inline proc >=(a: c_string, b: c_string) {
    return (__primitive("string_compare", a, b) >= 0);
  }

  inline proc <(a: c_string, b: c_string) {
    return (__primitive("string_compare", a, b) < 0);
  }

  inline proc >(a: c_string, b: c_string) {
    return (__primitive("string_compare", a, b) > 0);
  }

  inline proc =(ref a: c_string, b: c_string) {
    __primitive("=", a, b);
  }

  // let us set c_strings to NULL
  inline proc =(ref a:c_string, b:_nilType) { a = c_nil:c_string; }

  // for a to be a valid c_string after this function it must be on the same
  // locale as b
  inline proc =(ref a: c_string, b: string) {
    __primitive("=", a, b.c_str());
  }

  extern proc chpl_bool_to_c_string(x:bool) : c_string;
  inline proc _cast(type t, x: bool(?w)) where t == c_string {
    return chpl_bool_to_c_string(x:bool);
  }

  //
  // casts from c_string to c_void_ptr
  //
  inline proc _cast(type t, x: c_string) where t == c_void_ptr {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_void_ptr to c_string
  //
  inline proc _cast(type t, x: c_void_ptr) where t == c_string {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to bool types
  //
  inline proc _cast(type t, x:c_string) where isBoolType(t)
  {
    pragma "insert line file info"
    extern proc c_string_to_chpl_bool(x:c_string) : bool;
    return c_string_to_chpl_bool((x:string).strip().c_str()) : t;
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
    return c_string_to_int8_t((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == int(16)
    return c_string_to_int16_t((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == int(32)
    return c_string_to_int32_t((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == int(64)
    return c_string_to_int64_t((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == uint(8)
    return c_string_to_uint8_t((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == uint(16)
    return c_string_to_uint16_t((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == uint(32)
    return c_string_to_uint32_t((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == uint(64)
    return c_string_to_uint64_t((x:string).strip().c_str());

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
    return c_string_to_real32((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == real(64)
    return c_string_to_real64((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == imag(32)
    return c_string_to_imag32((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == imag(64)
    return c_string_to_imag64((x:string).strip().c_str());

  //
  // casts from c_string to complex types
  //
  pragma "insert line file info"
  extern proc c_string_to_complex64(x:c_string) : complex(64);
  pragma "insert line file info"
  extern proc c_string_to_complex128(x:c_string) : complex(128);

  inline proc _cast(type t, x:c_string) where t == complex(64)
    return c_string_to_complex64((x:string).strip().c_str());
  inline proc _cast(type t, x:c_string) where t == complex(128)
    return c_string_to_complex128((x:string).strip().c_str());

  //
  // casts from complex
  //
  inline proc _cast(type t, x: complex(?w)) where t == c_string {
    if isnan(x.re) || isnan(x.im) then
      return __primitive("string_copy", "nan");
    var re = (x.re):c_string;
    var im: c_string;
    var op: c_string;
    if x.im < 0 {
      im = (-x.im):c_string;
      op = " - ";
    } else if im == "-0.0" {
      im = "0.0":c_string;
      op = " - ";
    } else {
      im = (x.im):c_string;
      op = " + ";
    }
    // TODO: Add versions of the concatenation operator that consume their
    // c_string arg or args.
    const ts0 = re + op;
    chpl_free_c_string(re);
    const ts1 = ts0 + im;
    chpl_free_c_string(ts0);
    chpl_free_c_string(im);
    const ret = ts1 + "i";
    chpl_free_c_string(ts1);
    return ret;
  }

  extern proc real_to_c_string(x:real(64), isImag: bool) : c_string;
  //
  // casts from real
  //
  inline proc _cast(type t, x:real(?w)) where t == c_string {
    return real_to_c_string(x:real(64), false);
  }

  //
  // casts from imag
  //
  inline proc _cast(type t, x:imag(?w)) where t == c_string {
    // The Chapel version of the imag --> real cast smashes it flat rather than
    // just stripping off the "i".  See ChapelBase:965.
    var r = __primitive("cast", real(64), x);
    return real_to_c_string(r, true);
  }

  //
  // primitive c_string functions and methods
  //
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

  pragma "last resort" // avoids param string to c_string coercion
  inline proc param c_string.length param
    return __primitive("string_length", this);
  pragma "last resort" // avoids param string to c_string coercion
  inline proc _string_contains(param a: c_string, param b: c_string) param
    return __primitive("string_contains", a, b);

  /* Returns the index of the first occurrence of a substring within a string,
     or 0 if the substring is not in the string.
  */
  inline proc c_string.indexOf(substring:c_string):int
    return string_index_of(this, substring);
  extern proc string_index_of(haystack:c_string, needle:c_string):int;

  // Use with care.  Not for the weak.
  inline proc chpl_free_c_string(ref cs: c_string) {
    pragma "insert line file info"
    extern proc chpl_rt_free_c_string(ref cs: c_string);
    if (cs != c_nil:c_string) then chpl_rt_free_c_string(cs);
    // cs = c_nil;
  }

  proc c_string.writeThis(x) {
    compilerError("Cannot write a c_string, cast to a string first.");
  }

  proc c_string.readThis(x) {
    compilerError("Cannot read a c_string, use string.");
  }

}

