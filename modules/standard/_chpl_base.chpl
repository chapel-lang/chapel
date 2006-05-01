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


//
// bit manipulation functions
//
// note we need an init function so u1, u3, and u7 can be computed
// once and not every time bpop is called
//
fun bpop(i : int) {
  var u1 = 0u, u3 = 0u, u7 = 0u, tmp = 0u;

  u1 = 1;
  do {
    tmp = u1;
    u1 = 1 + (u1 << 3);
  } while u1 != tmp;
  u3 = 3;
  do {
    tmp = u3;
    u3 = 3 + (u3 << 3);
  } while u3 != tmp;
  u7 = 7;
  do {
    tmp = u7;
    u7 = 7 + (u7 << 9);
  } while u7 != tmp;
  tmp = i - ((i >> 1) & u3) - ((i >> 2) & u1);
  return (((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511;
}
