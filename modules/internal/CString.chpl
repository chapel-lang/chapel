/*
 * Copyright 2004-2019 Cray Inc.
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
  inline proc _defaultOf(type t:c_string) return c_nil:c_string;

  //inline proc c_string.c_str() return this;

  pragma "init copy fn"
  inline proc chpl__initCopy(x: c_string) : c_string {
    return x;
  }

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
  inline proc _cast(type t:c_string, x: chpl_anybool) {
    compilerWarning("cast from bool to c_string is deprecated");
    return chpl_bool_to_c_string(x:bool);
  }

  //
  // casts from c_string to c_void_ptr
  //
  inline proc _cast(type t:c_void_ptr, x: c_string) {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_void_ptr to c_string
  //
  inline proc _cast(type t:c_string, x: c_void_ptr) {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to bool types
  //
  inline proc _cast(type t:chpl_anybool, x:c_string) throws
    return try ((x:string).strip()): t;

  //
  // casts from c_string to integer types
  //
  inline proc _cast(type t:integral, x:c_string) throws
    return try ((x:string).strip()): t;

  //
  // casts from c_string to real/imag types
  //
  inline proc _cast(type t:chpl_anyreal, x:c_string) throws
    return try ((x:string).strip()): t;
  inline proc _cast(type t:chpl_anyimag, x:c_string) throws
    return try ((x:string).strip()): t;

  //
  // casts from c_string to complex types
  //
  inline proc _cast(type t:chpl_anycomplex, x:c_string) throws
    return try ((x:string).strip()): t;

  extern proc real_to_c_string(x:real(64), isImag: bool) : c_string;
  //
  // casts from real
  //
  inline proc _cast(type t:c_string, x:chpl_anyreal) {
    compilerWarning("cast from real to c_string is deprecated");
    return real_to_c_string(x:real(64), false);
  }

  //
  // casts from imag
  //
  inline proc _cast(type t:c_string, x:chpl_anyimag) {
    compilerWarning("cast from imag to c_string is deprecated");
    // The Chapel version of the imag --> real cast smashes it flat rather than
    // just stripping off the "i".  See ChapelBase:965.
    var r = __primitive("cast", real(64), x);
    return real_to_c_string(r, true);
  }

  //
  // casts from integral
  //
  proc _cast(type t:c_string, x: integral) {
    compilerWarning("cast from integral to c_string is deprecated");
    extern proc integral_to_c_string(x:int(64), size:uint(32), isSigned: bool, ref err: bool) : c_string;

    var isErr: bool;
    var csc = integral_to_c_string(x:int(64), numBytes(x.type), isIntType(x.type), isErr);

    // this should only happen if the runtime is broken
    if isErr {
      try! {
        throw new unmanaged IllegalArgumentError("Unexpected case in integral_to_c_string");
      }
    }

    return csc;
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

