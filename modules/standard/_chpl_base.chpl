// _chpl_base module

class _unused_class { } // for interfacing with analysis (sym_tuple)


//
// Setter and method tokens
//
record _methodTokenType { }
var _methodToken : _methodTokenType;
record _setterTokenType { }
var _setterToken : _setterTokenType;


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

pragma "inline" function length(a: string) return __primitive("string_length", a);

pragma "inline" function string.this(i: integer) return __primitive("string_index", this, i);


//
// More primitive functions
//
pragma "no codegen" pragma "rename _chpl_exit" function exit(status : integer) {
  __primitive("exit");       
}

pragma "no codegen" function halt() {
  __primitive("halt");
}

pragma "no codegen" function assert() {
  __primitive("assert");
}

function init_elts(x, s, e) {
  for i in 1..s {
    var y : e;
    x(i) = y;
  }
}

// dynamic data block class
pragma "data class"
class _ddata {
  type elt_type;
  var size : integer;
  var _element : elt_type;
  pragma "inline" function init() {
    __primitive("array_init", this, size, _element);
    init_elts(this, size, _element);
  }
  pragma "inline" function this(i : integer) : elt_type {
    return __primitive("array_index", this, i);
  }
  pragma "inline" function =this(i : integer, val : elt_type) {
    __primitive("array_set", this, i, val);
  }
}


//
// MOVE THESE, maybe to a memory module?
//
pragma "no codegen" function _chpl_memtest_printMemTable();
pragma "no codegen" function _chpl_memtest_printMemStat();
pragma "no codegen" function _chpl_memtest_resetMemStat();
pragma "no codegen" function _chpl_memtest_allocAndFree();
pragma "no codegen" function _chpl_memtest_freedMalloc();
pragma "no codegen" function _chpl_memtest_freedWithoutMalloc();
pragma "no codegen" function _chpl_memtest_reallocWithoutMalloc();
pragma "no codegen" function _chpl_memtest_reallocZeroSize();
pragma "no codegen" function _chpl_memtest_mallocOutOfMemory();
pragma "no codegen" function _chpl_memtest_reallocOutOfMemory();

pragma "no codegen" function startTrackingMem();
