/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
  private use ChapelStandard, SysCTypes;
  private use CPtr;

  //inline proc c_string.c_str() return this;

  pragma "init copy fn"
  inline proc chpl__initCopy(x: c_string, definedConst: bool) : c_string {
    return x;
  }

  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: c_string, definedConst: bool) : c_string {
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

  //
  // casts from nil to c_string
  //
  inline operator :(x: _nilType, type t:c_string) {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to c_void_ptr
  //
  inline operator :(x: c_string, type t:c_void_ptr) {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_void_ptr to c_string
  //
  inline operator :(x: c_void_ptr, type t:c_string) {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to c_ptr(c_char/int(8)/uint(8))
  //
  inline operator :(x: c_string, type t:c_ptr)
    where t.eltType == c_char || t.eltType == int(8) || t.eltType == uint(8)
  {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_ptr(c_char/int(8)/uint(8)) to c_string
  //
  inline operator :(x: c_ptr, type t:c_string)
    where x.eltType == c_char || x.eltType == int(8) || x.eltType == uint(8)
  {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to bool types
  //
  inline operator :(x:c_string, type t:chpl_anybool) throws {
    var chplString: string;
    try! {
      chplString = createStringWithNewBuffer(x);
    }
    return try (chplString.strip()): t;
  }

  //
  // casts from c_string to integer types
  //
  inline operator :(x:c_string, type t:integral) throws {
    var chplString: string;
    try! {
      chplString = createStringWithNewBuffer(x);
    }
    return try (chplString.strip()): t;
  }

  //
  // casts from c_string to real/imag types
  //
  inline operator :(x:c_string, type t:chpl_anyreal)  throws {
    var chplString: string;
    try! {
      chplString = createStringWithNewBuffer(x);
    }
    return try (chplString.strip()): t;
  }

  inline operator :(x:c_string, type t:chpl_anyimag) throws {
    var chplString: string;
    try! {
      chplString = createStringWithNewBuffer(x);
    }
    return try (chplString.strip()): t;
  }

  //
  // casts from c_string to complex types
  //
  inline operator :(x:c_string, type t:chpl_anycomplex)  throws {
    var chplString: string;
    try! {
      chplString = createStringWithNewBuffer(x);
    }
    return try (chplString.strip()): t;
  }

  //
  // primitive c_string functions and methods
  //

  inline proc c_string.size return __primitive("string_length_bytes", this);

  inline proc c_string.substring(i: int)
    return __primitive("string_index", this, i);

  inline proc c_string.substring(r: range(?)) {
    var r2 = r[1..this.size];  // This may warn about ambiguously aligned ranges.
    var lo:int = r2.alignedLow, hi:int = r2.alignedHigh;
    return __primitive("string_select", this, lo, hi, r2.stride);
  }

  pragma "last resort" // avoids param string to c_string coercion
  inline proc param c_string.size param {
    return __primitive("string_length_bytes", this);
  }
  pragma "last resort" // avoids param string to c_string coercion
  inline proc _string_contains(param a: c_string, param b: c_string) param
    return __primitive("string_contains", a, b);

  /* Returns the index of the first occurrence of a substring within a string,
     or 0 if the substring is not in the string.
  */
  inline proc c_string.indexOf(substring:c_string):int
    return string_index_of(this, substring);

  pragma "fn synchronization free"
  extern proc string_index_of(haystack:c_string, needle:c_string):int;

  // Use with care.  Not for the weak.
  inline proc chpl_free_c_string(ref cs: c_string) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_rt_free_c_string(ref cs: c_string);
    if (cs != c_nil:c_string) then chpl_rt_free_c_string(cs);
    // cs = c_nil;
  }

  proc c_string.writeThis(x) throws {
    compilerError("Cannot write a c_string, cast to a string first.");
  }

  proc c_string.readThis(x) throws {
    compilerError("Cannot read a c_string, use string.");
  }

}

