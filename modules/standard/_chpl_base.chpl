// _chpl_base module

class _unused_class { } // for interfacing with analysis (sym_tuple)

//
// Primitive functions for numeric types
//
pragma "inline" function -(a: numeric) return __primitive("u-", a);
pragma "inline" function +(a: numeric) return __primitive("u+", a);
pragma "inline" function ~(a: numeric) return __primitive("u~", a);
pragma "inline" function not(a: numeric) return __primitive("not", a);
pragma "inline" function +(a: numeric, b: numeric) return __primitive("+", a, b);
pragma "inline" function -(a: numeric, b: numeric) return __primitive("-", a, b);
pragma "inline" function *(a: numeric, b: numeric) return __primitive("*", a, b);
pragma "inline" function /(a: numeric, b: numeric) return __primitive("/", a, b);
pragma "inline" function mod(a: numeric, b: numeric) return __primitive("%", a, b);
pragma "inline" function ==(a: numeric, b: numeric) return __primitive("==", a, b); 
pragma "inline" function !=(a: numeric, b: numeric) return __primitive("!=", a, b);
pragma "inline" function <=(a: numeric, b: numeric) return __primitive("<=", a, b);
pragma "inline" function >=(a: numeric, b: numeric) return __primitive(">=", a, b);
pragma "inline" function <(a: numeric, b: numeric) return __primitive("<", a, b);
pragma "inline" function >(a: numeric, b: numeric) return __primitive(">", a, b);
pragma "inline" function &(a: numeric, b: numeric) return __primitive("&", a, b);
pragma "inline" function |(a: numeric, b: numeric) return __primitive("|", a, b);
pragma "inline" function ^(a: numeric, b: numeric) return __primitive("^", a, b);
pragma "inline" function and(a: numeric, b: numeric) return __primitive("&&", a, b);
pragma "inline" function or(a: numeric, b: numeric) return __primitive("||", a, b);
pragma "inline" function **(a: numeric, b: numeric) return __primitive("**", a, b);

//
// Primitive functions for objects
//
pragma "inline" function ==(a: object, b: object) : boolean return __primitive("ptr_eq", a, b);
pragma "inline" function !=(a: object, b: object) : boolean return __primitive("ptr_neq", a, b);


//
// Primitive functions for strings
//
pragma "inline" function ==(a: string, b: string) : boolean return __primitive("string_equal", a, b);

pragma "inline" function !=(a: string, b: string) : boolean return not a == b;

pragma "inline" function +(a: string, b: string) : string return __primitive("string_concat", a, b);

pragma "rename inline" function length(a: string) return __primitive("string_length", a);

pragma "inline" function string.this(i: integer) return __primitive("string_index", this, i);

pragma "inline" function string.this(s: _aseq)
  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);
