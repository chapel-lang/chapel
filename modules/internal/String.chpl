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

pragma "no use ChapelStandard"

// Chapel Strings
module String {
  use CString;

  pragma "default string value" extern var defaultStringValue: string = noinit;

  // The following method is called by the compiler to determine the default
  // value of a given type.  For strings, this should return the value
  // defined above.
  // strings will be records, so param is not possible for them
  inline proc _defaultOf(type t) where t: string return defaultStringValue;

  // String concatenation
  inline proc +(s: string, x: string) {
    const cs = __primitive("string_concat", s.c_str(), x.c_str());
    // FIX ME: could use a toString() that doesn't allocate space
    const ret = toString(cs);
    chpl_free_c_string(cs);
    return ret;
  }

  inline proc +(s: c_string, x: string) {
    const cs = __primitive("string_concat", s, x.c_str());
    // FIX ME: could use a toString() that doesn't allocate space
    const ret = toString(cs);
    chpl_free_c_string(cs);
    return ret;
  }

  inline proc +(s: string, x: c_string) {
    const cs = __primitive("string_concat", s.c_str(), x);
    // FIX ME: could use a toString() that doesn't allocate space
    const ret = toString(cs);
    chpl_free_c_string(cs);
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
    const cs = this.c_str().substring(i);
    // FIX ME: could use a toString() that doesn't allocate space
    const ret = toString(cs);
    if cs.length != 0 then chpl_free_c_string(cs);
    return ret;
  }
  inline proc string.substring(r: range(?)) {
    const cs = this.c_str().substring(r);
    // FIX ME: could use a toString() that doesn't allocate space
    const ret = toString(cs);
    if cs.length != 0 then chpl_free_c_string(cs);
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
  inline proc _cast(type t, x) where t==string && x.type != c_string {
    // Caution: The result of a cast to c_string does not necessarily own the string data
    // it contains.  Therefore it must not be freed.
    // TODO: Rework the interface to _cast(c_string, x) so it guarantees one or
    // the other.
    const cs = _cast(c_string, x);
    const ret = toString(cs);
    // This free is not safe because ownership of the c_string returned by the
    // _cast above is ambiguous.
    //    chpl_free_c_string(cs);
    return ret;
  }

  pragma "compiler generated"
  inline proc _cast(type t, x: string) where t !=c_string
    return _cast(t, x.c_str());
  
  inline proc _cast(type t, x: c_string) where _isPrimitiveType(t) && t!=string
    return __primitive("cast", t, x);

  //
  // casts to complex
  //
  inline proc _cast(type t, x: c_string) where isComplexType(t)
    return __primitive("cast", t, x);
  
  //
  // casts to imag
  //
  inline proc _cast(type t, x: c_string) where isImagType(t)
    return __primitive("cast", t, x);
  
  //
  // casts from complex
  //
  inline proc _cast(type t, x: complex(?w)) where t == c_string {
    if isnan(x.re) || isnan(x.im) then
      return "nan";
    var re = (x.re):c_string;
    var im, op: c_string;
    if x.im < 0 {
      im = (-x.im):c_string;
      op = " - ";
    } else if im == "-0.0" {
      im = "0.0";
      op = " - ";
    } else {
      im = (x.im):c_string;
      op = " + ";
    }
    const ts0 = re + op;
    chpl_free_c_string(re);
    const ts1 = ts0 + im;
    chpl_free_c_string(ts0);
    if im != "-0.0" then chpl_free_c_string(im);
    const ret = ts1 + "i":c_string;
    chpl_free_c_string(ts1);
    return ret;
  }
  
  //
  // casts from imag
  //
  inline proc _cast(type t, x: imag(?w)) where t == c_string
    return __primitive("cast", t, x);

  pragma "compiler generated"
  pragma "init copy fn"
  inline proc chpl__initCopy(a) {
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
//extern type c_string; is a built-in primitive type
//
// In terms of how they are used, c_strings are a "close to the metal"
// representation, being in essence the common NUL-terminated C string.
//
// Memory management caveat: In general, an object of type c_string does not
// own its data (that is, some do, but not all).  Therefore it is only safe to
// free a c_string object if you know its provenance, and can trace this back
// to a string-allocating function such as string_concat(),
// chpl_glom_strings() or string_copy().
module CString {

  // The following method is called by the compiler to determine the default
  // value of a given type.
  inline proc _defaultOf(type t) param where t: c_string return "":c_string;

  inline proc toString(cstr:c_string):string {
    return __primitive("string_from_c_string", cstr, 0, 0);
  }
  inline proc toString(cstr:c_string, len:int):string {
    return __primitive("string_from_c_string", cstr, 1, len);
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

  // DANGER! Memory is shared between c_strings assigned thus.  Only one (at
  // most) can be safely freed.
  inline proc =(ref a: c_string, b: c_string) {
    __primitive("=", a, b);
  }

  // DANGER! The c_string on the LHS does not own the string it contains after
  // this assignment.
  inline proc =(ref a: c_string, b: string) {
    __primitive("=", a, b.c_str());
  }

  inline proc =(ref a: string, b: c_string) {
    __primitive("=", a, toString(b));
  }

  inline proc _cast(type t, x: c_string) where t == string {
    return toString(x);
  }

  // DANGER, the result of this cast is an unowned c_string.
  inline proc _cast(type t, x: string) where t == c_string {
    return x.c_str();
  }

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

  proc ref c_string.writeThis(x: Writer) {
    x.write(this); // FIX ME? toString
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
    if r2.isEmpty() then return "";
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
  inline proc chpl_free_c_string(cs: c_string) {
    pragma "insert line file info"
    extern proc chpl_rt_free_c_string(cs: c_string);
    if cs.length > 0 then chpl_rt_free_c_string(cs);
  }

}

