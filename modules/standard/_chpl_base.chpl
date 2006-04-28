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
// Primitive funs for numeric types
//
pragma "inline" fun -(a: numeric) return __primitive("u-", a);
pragma "inline" fun +(a: numeric) return __primitive("u+", a);
pragma "inline" fun ~(a: numeric) return __primitive("u~", a);
pragma "inline" fun !(a: numeric) return __primitive("!", a);
pragma "inline" fun +(a: numeric, b: numeric) return __primitive("+", a, b);
pragma "inline" fun -(a: numeric, b: numeric) return __primitive("-", a, b);
pragma "inline" fun *(a: numeric, b: numeric) return __primitive("*", a, b);
pragma "inline" fun /(a: numeric, b: numeric) return __primitive("/", a, b);
pragma "inline" fun %(a: numeric, b: numeric) return __primitive("%", a, b);
pragma "inline" fun <<(a: numeric, b: numeric) return __primitive("<<", a, b);
pragma "inline" fun >>(a: numeric, b: numeric) return __primitive(">>", a, b);
pragma "inline" fun ==(a: numeric, b: numeric) return __primitive("==", a, b); 
pragma "inline" fun !=(a: numeric, b: numeric) return __primitive("!=", a, b);
pragma "inline" fun <=(a: numeric, b: numeric) return __primitive("<=", a, b);
pragma "inline" fun >=(a: numeric, b: numeric) return __primitive(">=", a, b);
pragma "inline" fun <(a: numeric, b: numeric) return __primitive("<", a, b);
pragma "inline" fun >(a: numeric, b: numeric) return __primitive(">", a, b);
pragma "inline" fun &(a: numeric, b: numeric) return __primitive("&", a, b);
pragma "inline" fun |(a: numeric, b: numeric) return __primitive("|", a, b);
pragma "inline" fun ^(a: numeric, b: numeric) return __primitive("^", a, b);
pragma "inline" fun &&(a: numeric, b: numeric) return __primitive("&&", a, b);
pragma "inline" fun ||(a: numeric, b: numeric) return __primitive("||", a, b);
pragma "inline" fun **(a: numeric, b: numeric) return __primitive("**", a, b);


//
// Primitive math funs
//

pragma "no codegen" pragma "rename sin" fun sin(x: float) : float {
  return __primitive("sin", x);
}


//
// Primitive funs for objects
//
pragma "inline" fun ==(a: object, b: object) : bool return __primitive("ptr_eq", a, b);
pragma "inline" fun !=(a: object, b: object) : bool return __primitive("ptr_neq", a, b);


//
// Primitive funs for strings
//
pragma "inline" fun ==(a: string, b: string) : bool return __primitive("string_equal", a, b);

pragma "inline" fun !=(a: string, b: string) : bool return ! a == b;

pragma "inline" fun +(a: string, b: string) : string return __primitive("string_concat", a, b);

pragma "inline" fun length(a: string) return __primitive("string_length", a);

pragma "inline" fun string.this(i: int) return __primitive("string_index", this, i);


//
// More primitive funs
//
pragma "no codegen" pragma "rename _chpl_exit" fun exit(status : int) {
  __primitive("exit");       
}

pragma "no codegen" pragma "rename fflush" fun fflush(fp: CFILEPTR) : int {
  return __primitive("fflush", fp.FILEptr);
}

fun init_elts(x, s, e) {
  for i in 1..s {
    var y : e;
    x(i-1) = y;
  }
}

// dynamic data block class
pragma "data class"
class _ddata {
  type elt_type;
  var size : int;
  var _element : elt_type;
  pragma "inline" fun init() {
    __primitive("array_init", this, size, _element);
    init_elts(this, size, _element);
  }
  pragma "inline" fun this(i : int) : elt_type {
    return __primitive("array_index", this, i);
  }
  pragma "inline" fun =this(i : int, val : elt_type) {
    __primitive("array_set", this, i, val);
  }
}

//
// MOVE THESE, maybe to a memory module?
//
pragma "no codegen" fun _chpl_memtest_printMemTable();
pragma "no codegen" fun _chpl_memtest_printMemStat();
pragma "no codegen" fun _chpl_memtest_resetMemStat();
pragma "no codegen" fun _chpl_memtest_allocAndFree();
pragma "no codegen" fun _chpl_memtest_freedMalloc();
pragma "no codegen" fun _chpl_memtest_freedWithoutMalloc();
pragma "no codegen" fun _chpl_memtest_reallocWithoutMalloc();
pragma "no codegen" fun _chpl_memtest_reallocZeroSize();
pragma "no codegen" fun _chpl_memtest_mallocOutOfMemory();
pragma "no codegen" fun _chpl_memtest_reallocOutOfMemory();

pragma "no codegen" fun startTrackingMem();
