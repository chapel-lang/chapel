pragma "no use ChapelStandard"

// Chapel Strings
module String {
  use CString;

  pragma "default string value" extern var defaultStringValue: string = noinit;

  // String concatenation
  inline proc +(s: string, x: string)
    // FIX ME: leak c_string
    return toString(__primitive("string_concat", s.c_str(), x.c_str()));

  pragma "compiler generated"
  inline proc +(s: c_string, x: string)
    // FIX ME: leak c_string
    return toString(__primitive("string_concat", s, x.c_str()));

  pragma "compiler generated"
  inline proc +(s: string, x: c_string)
    // FIX ME: leak c_string
    return toString(__primitive("string_concat", s.c_str(), x));

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
  inline proc +(param a: string, param b: string) param return __primitive("string_concat", a, b);

  //
  // primitive string functions and methods
  //
  inline proc ascii(a: string) return ascii(a.c_str());
  inline proc string.length return this.c_str().length;
  inline proc string.size return this.length;
  inline proc string.substring(i: int)
    // FIX ME: leak c_string
    return toString(this.c_str().substring(i));
  inline proc _string_contains(a: string, b: string)
    return _string_contains(a.c_str(), b.c_str());
  
  /* Returns the index of the first occurrence of a substring within a string,
      or 0 if the substring is not in the string.
   */
  inline proc string.indexOf(substring:string):int
    return this.c_str().indexOf(substring.c_str());
  inline proc string.indexOf(substring:c_string):int
    return this.c_str().indexOf(substring);
  
  // cast to and from Chapel strings use c_string
  pragma "compiler generated"
  inline proc _cast(type t, x) where t==string && x.type != c_string
    // FIX ME: leak c_string
    return toString(_cast(c_string, x));

  pragma "compiler generated"
  inline proc _cast(type t, x: string) where t !=c_string
    return _cast(t, x.c_str());
  
  inline proc _cast(type t, x: c_string) where _isPrimitiveType(t) && t!=string
    return __primitive("cast", t, x);

  //
  // casts to complex
  //
  inline proc _cast(type t, x: c_string) where _isComplexType(t)
    return __primitive("cast", t, x);
  
  //
  // casts to imag
  //
  inline proc _cast(type t, x: c_string) where _isImagType(t)
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
    return (re + op + im + "i").c_str();
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
module CString {

  inline proc toString(cstr:c_string):string {
    return __primitive("string_from_c_string", cstr, 0, 0);
  }
  inline proc toString(cstr:c_string, len:int):string {
    return __primitive("string_from_c_string", cstr, 1, len);
  }

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
    __primitive("=", a, __primitive("c_string_from_string", b));
  }

  inline proc =(ref a: string, b: c_string) {
    __primitive("=", a, __primitive("string_from_c_string", b, 0, 0));
  }

  inline proc _cast(type t, x: c_string) where t == string {
    return __primitive("string_from_c_string", x, 0, 0);
  }

  inline proc _cast(type t, x: string) where t == c_string {
    return __primitive("c_string_from_string", x);
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

}

