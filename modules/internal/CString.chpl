/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
  private use ChapelStandard, CTypes;

  //inline proc c_string.c_str() return this;

  pragma "init copy fn"
  pragma "last resort" // attempt to prevent c_ptrConst(c_char) converting
  inline proc chpl__initCopy(x: chpl_c_string, definedConst: bool) : chpl_c_string {
    return x;
  }

  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: chpl_c_string, definedConst: bool) : chpl_c_string {
    return x;
  }

  inline operator chpl_c_string.==(s0: chpl_c_string, s1: chpl_c_string) {
    return __primitive("string_compare", s0, s1) == 0;
  }

//  inline operator c_string.==(s0: string, s1: c_string) {
//    return __primitive("string_compare", s0.c_str(), s1) == 0;
//  }
//
//  inline operator ==(s0: c_string, s1: string) {
//    return __primitive("string_compare", s0, s1.c_str()) == 0;
//  }

  inline operator chpl_c_string.!=(s0: chpl_c_string, s1: chpl_c_string) {
    return __primitive("string_compare", s0, s1) != 0;
  }

//  inline operator c_string.!=(s0: string, s1: c_string) {
//    return __primitive("string_compare", s0.c_str(), s1) != 0;
//  }
//
//  inline operator !=(s0: c_string, s1: string) {
//    return __primitive("string_compare", s0, s1.c_str()) != 0;
//  }

  inline operator chpl_c_string.<=(a: chpl_c_string, b: chpl_c_string) {
    return (__primitive("string_compare", a, b) <= 0);
  }

  inline operator chpl_c_string.>=(a: chpl_c_string, b: chpl_c_string) {
    return (__primitive("string_compare", a, b) >= 0);
  }

  inline operator chpl_c_string.<(a: chpl_c_string, b: chpl_c_string) {
    return (__primitive("string_compare", a, b) < 0);
  }

  inline operator chpl_c_string.>(a: chpl_c_string, b: chpl_c_string) {
    return (__primitive("string_compare", a, b) > 0);
  }

  inline operator chpl_c_string.=(ref a: chpl_c_string, b: chpl_c_string) {
    __primitive("=", a, b);
  }

  // let us set c_strings to NULL
  inline operator chpl_c_string.=(ref a:chpl_c_string, b:_nilType) { a = nil:chpl_c_string; }

  // for a to be a valid c_string after this function it must be on the same
  // locale as b
  inline operator chpl_c_string.=(ref a: chpl_c_string, b: string) {
    __primitive("=", a, b.c_str());
  }

  //
  // casts from nil to c_string
  //
  inline operator :(x: _nilType, type t:chpl_c_string) {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to c_ptr(void)
  //
  inline operator :(x: chpl_c_string, type t:c_ptr(void)) {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_ptr(void) to c_string
  //
  inline operator :(x: c_ptr(void), type t:chpl_c_string) {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to c_ptr(c_char/int(8)/uint(8))
  //
  inline operator :(x: chpl_c_string, type t:c_ptr(?eltType))
    where eltType == c_char || eltType == int(8) || eltType == uint(8)
  {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_string to c_ptrConst(c_char/int(8)/uint(8))
  //
  inline operator :(x: chpl_c_string, type t:c_ptrConst(?eltType))
    where eltType == c_char || eltType == int(8) || eltType == uint(8)
  {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_ptr(c_char/int(8)/uint(8)) to c_string
  //
  inline operator :(x: c_ptr(?eltType), type t:chpl_c_string)
    where eltType == c_char || eltType == int(8) || eltType == uint(8)
  {
    return __primitive("cast", t, x);
  }
  //
  // casts from c_ptrConst(c_char/int(8)/uint(8)) to c_string
  //
  inline operator :(x: c_ptrConst(?eltType), type t:chpl_c_string)
    where eltType == c_char || eltType == int(8) || eltType == uint(8)
  {
    return __primitive("cast", t, x);
  }

  //
  // casts from c_string to bool types
  //
  inline operator :(x:chpl_c_string, type t:bool) throws {
    var chplString: string;
    try! {
      chplString = string.createCopyingBuffer(x:c_ptrConst(c_char));
    }
    return try (chplString.strip()): t;
  }

  //
  // casts from c_string to integer types
  //
  inline operator :(x:chpl_c_string, type t:integral) throws {
    var chplString: string;
    try! {
      chplString = string.createCopyingBuffer(x:c_ptrConst(c_char));
    }
    return try (chplString.strip()): t;
  }

  //
  // casts from c_string to real/imag types
  //
  inline operator :(x:chpl_c_string, type t:chpl_anyreal)  throws {
    var chplString: string;
    try! {
      chplString = string.createCopyingBuffer(x:c_ptrConst(c_char));
    }
    return try (chplString.strip()): t;
  }

  inline operator :(x:chpl_c_string, type t:chpl_anyimag) throws {
    var chplString: string;
    try! {
      chplString = string.createCopyingBuffer(x:c_ptrConst(c_char));
    }
    return try (chplString.strip()): t;
  }

  //
  // casts from c_string to complex types
  //
  inline operator :(x:chpl_c_string, type t:chpl_anycomplex)  throws {
    var chplString: string;
    try! {
      chplString = string.createCopyingBuffer(x:c_ptrConst(c_char));
    }
    return try (chplString.strip()): t;
  }

  //
  // primitive c_string functions and methods
  //

  inline proc chpl_c_string.size do return __primitive("string_length_bytes", this);

  inline proc chpl_c_string.substring(i: int) do
    return __primitive("string_index", this, i);

  inline proc chpl_c_string.substring(r: range(?)) {
    var r2 = r[1..this.size];  // This may warn about ambiguously aligned ranges.
    var lo:int = r2.low, hi:int = r2.high;
    return __primitive("string_select", this, lo, hi, r2.stride);
  }

  pragma "last resort" // avoids param string to c_string coercion
  inline proc param chpl_c_string.size param {
    return __primitive("string_length_bytes", this);
  }
  pragma "last resort" // avoids param string to c_string coercion
  inline proc _string_contains(param a: chpl_c_string, param b: chpl_c_string) param do
    return __primitive("string_contains", a, b);

  /* Returns the index of the first occurrence of a substring within a string,
     or 0 if the substring is not in the string.
  */
  inline proc chpl_c_string.indexOf(substring:chpl_c_string):int do
    return string_index_of(this, substring);

  pragma "fn synchronization free"
  extern proc string_index_of(haystack:chpl_c_string, needle:chpl_c_string):int;

  // Use with care.  Not for the weak.
  inline proc chpl_free_c_string(ref cs: chpl_c_string) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_rt_free_c_string(ref cs: chpl_c_string);
    if (cs != nil:chpl_c_string) then chpl_rt_free_c_string(cs);
    // cs = nil;
  }

  proc chpl_c_string.serialize(writer, ref serializer) throws {
    compilerError("Cannot write a c_string, use one of the 'string.create*ingBuffer' methods to create a string first.");
  }

  proc chpl_c_string.deserialize(reader, ref deserializer) throws {
    compilerError("Cannot read a c_string, use string.");
  }

}

