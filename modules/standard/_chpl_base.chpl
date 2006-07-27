// _chpl_base module

class object { }
record value { }

var _aobject : object;
var _avalue : value;

pragma "inline" fun _chpl_alloc(type t, description:string)
  return __primitive("chpl_alloc", t, description);

// Primitive functions and operators on bool
pragma "inline" fun =(a: bool, b: bool) return b;
//pragma "inline" fun _copy(a: bool) return a;
pragma "inline" fun ~(a: bool) return __primitive("u~", a);
pragma "inline" fun &(a: bool, b: bool) return __primitive("&", a, b);
pragma "inline" fun |(a: bool, b: bool) return __primitive("|", a, b);
pragma "inline" fun ^(a: bool, b: bool) return __primitive("^", a, b);
pragma "inline" fun ==(a: bool, b: bool) return __primitive("==", a, b);
pragma "inline" fun !=(a: bool, b: bool) return __primitive("!=", a, b);
pragma "inline" fun <=(a: bool, b: bool) return __primitive("<=", a, b);
pragma "inline" fun >=(a: bool, b: bool) return __primitive(">=", a, b);
pragma "inline" fun <(a: bool, b: bool) return __primitive("<", a, b);
pragma "inline" fun >(a: bool, b: bool) return __primitive(">", a, b);
pragma "inline" fun !(a: bool) return __primitive("!", a);
pragma "inline" fun &&(a: bool, b: bool) return __primitive("&&", a, b);
pragma "inline" fun ||(a: bool, b: bool) return __primitive("||", a, b);

// Primitive functions and operators on int(8)
pragma "inline" fun =(a: int(8), b: int) return b:int(8);
//pragma "inline" fun _copy(a: int(8)) return a;
pragma "inline" fun +(a: int(8)) return __primitive("u+", a);
pragma "inline" fun -(a: int(8)) return __primitive("u-", a);
pragma "inline" fun +(a: int(8), b: int(8)) return __primitive("+", a, b);
pragma "inline" fun -(a: int(8), b: int(8)) return __primitive("-", a, b);
pragma "inline" fun *(a: int(8), b: int(8)) return __primitive("*", a, b);
pragma "inline" fun /(a: int(8), b: int(8)) return __primitive("/", a, b);
pragma "inline" fun **(a: int(8), b: int(8)) return __primitive("**", a, b);
pragma "inline" fun %(a: int(8), b: int(8)) return __primitive("%", a, b);
pragma "inline" fun ~(a: int(8)) return __primitive("u~", a);
pragma "inline" fun &(a: int(8), b: int(8)) return __primitive("&", a, b);
pragma "inline" fun |(a: int(8), b: int(8)) return __primitive("|", a, b);
pragma "inline" fun ^(a: int(8), b: int(8)) return __primitive("^", a, b);
pragma "inline" fun <<(a: int(8), b: int(8)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: int(8), b: int(8)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: int(8), b: int(8)) return __primitive("==", a, b);
pragma "inline" fun !=(a: int(8), b: int(8)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: int(8), b: int(8)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: int(8), b: int(8)) return __primitive(">=", a, b);
pragma "inline" fun <(a: int(8), b: int(8)) return __primitive("<", a, b);
pragma "inline" fun >(a: int(8), b: int(8)) return __primitive(">", a, b);
pragma "inline" fun !(a: int(8)) return __primitive("!", a);
pragma "inline" fun &&(a: int(8), b: int(8)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: int(8), b: int(8)) return __primitive("||", a, b);

// Primitive functions and operators on int(16)
pragma "inline" fun =(a: int(16), b: int) return b:int(16);
//pragma "inline" fun _copy(a: int(16)) return a;
pragma "inline" fun +(a: int(16)) return __primitive("u+", a);
pragma "inline" fun -(a: int(16)) return __primitive("u-", a);
pragma "inline" fun +(a: int(16), b: int(16)) return __primitive("+", a, b);
pragma "inline" fun -(a: int(16), b: int(16)) return __primitive("-", a, b);
pragma "inline" fun *(a: int(16), b: int(16)) return __primitive("*", a, b);
pragma "inline" fun /(a: int(16), b: int(16)) return __primitive("/", a, b);
pragma "inline" fun **(a: int(16), b: int(16)) return __primitive("**", a, b);
pragma "inline" fun %(a: int(16), b: int(16)) return __primitive("%", a, b);
pragma "inline" fun ~(a: int(16)) return __primitive("u~", a);
pragma "inline" fun &(a: int(16), b: int(16)) return __primitive("&", a, b);
pragma "inline" fun |(a: int(16), b: int(16)) return __primitive("|", a, b);
pragma "inline" fun ^(a: int(16), b: int(16)) return __primitive("^", a, b);
pragma "inline" fun <<(a: int(16), b: int(16)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: int(16), b: int(16)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: int(16), b: int(16)) return __primitive("==", a, b);
pragma "inline" fun !=(a: int(16), b: int(16)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: int(16), b: int(16)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: int(16), b: int(16)) return __primitive(">=", a, b);
pragma "inline" fun <(a: int(16), b: int(16)) return __primitive("<", a, b);
pragma "inline" fun >(a: int(16), b: int(16)) return __primitive(">", a, b);
pragma "inline" fun !(a: int(16)) return __primitive("!", a);
pragma "inline" fun &&(a: int(16), b: int(16)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: int(16), b: int(16)) return __primitive("||", a, b);

// Primitive functions and operators on int(32)
pragma "inline" fun =(a: int(32), b: int) return b:int(32);
//pragma "inline" fun _copy(a: int(32)) return a;
pragma "inline" fun +(a: int(32)) return __primitive("u+", a);
pragma "inline" fun -(a: int(32)) return __primitive("u-", a);
pragma "inline" fun +(a: int(32), b: int(32)) return __primitive("+", a, b);
pragma "inline" fun -(a: int(32), b: int(32)) return __primitive("-", a, b);
pragma "inline" fun *(a: int(32), b: int(32)) return __primitive("*", a, b);
pragma "inline" fun /(a: int(32), b: int(32)) return __primitive("/", a, b);
pragma "inline" fun **(a: int(32), b: int(32)) return __primitive("**", a, b);
pragma "inline" fun %(a: int(32), b: int(32)) return __primitive("%", a, b);
pragma "inline" fun ~(a: int(32)) return __primitive("u~", a);
pragma "inline" fun &(a: int(32), b: int(32)) return __primitive("&", a, b);
pragma "inline" fun |(a: int(32), b: int(32)) return __primitive("|", a, b);
pragma "inline" fun ^(a: int(32), b: int(32)) return __primitive("^", a, b);
pragma "inline" fun <<(a: int(32), b: int(32)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: int(32), b: int(32)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: int(32), b: int(32)) return __primitive("==", a, b);
pragma "inline" fun !=(a: int(32), b: int(32)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: int(32), b: int(32)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: int(32), b: int(32)) return __primitive(">=", a, b);
pragma "inline" fun <(a: int(32), b: int(32)) return __primitive("<", a, b);
pragma "inline" fun >(a: int(32), b: int(32)) return __primitive(">", a, b);
pragma "inline" fun !(a: int(32)) return __primitive("!", a);
pragma "inline" fun &&(a: int(32), b: int(32)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: int(32), b: int(32)) return __primitive("||", a, b);

// Primitive functions and operators on int(64)
pragma "inline" fun =(a: int(64), b: int(64)) return b;
//pragma "inline" fun _copy(a: int(64)) return a;
pragma "inline" fun +(a: int(64)) return __primitive("u+", a);
pragma "inline" fun -(a: int(64)) return __primitive("u-", a);
pragma "inline" fun +(a: int(64), b: int(64)) return __primitive("+", a, b);
pragma "inline" fun -(a: int(64), b: int(64)) return __primitive("-", a, b);
pragma "inline" fun *(a: int(64), b: int(64)) return __primitive("*", a, b);
pragma "inline" fun /(a: int(64), b: int(64)) return __primitive("/", a, b);
pragma "inline" fun **(a: int(64), b: int(64)) return __primitive("**", a, b);
pragma "inline" fun %(a: int(64), b: int(64)) return __primitive("%", a, b);
pragma "inline" fun ~(a: int(64)) return __primitive("u~", a);
pragma "inline" fun &(a: int(64), b: int(64)) return __primitive("&", a, b);
pragma "inline" fun |(a: int(64), b: int(64)) return __primitive("|", a, b);
pragma "inline" fun ^(a: int(64), b: int(64)) return __primitive("^", a, b);
pragma "inline" fun <<(a: int(64), b: int(64)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: int(64), b: int(64)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: int(64), b: int(64)) return __primitive("==", a, b);
pragma "inline" fun !=(a: int(64), b: int(64)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: int(64), b: int(64)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: int(64), b: int(64)) return __primitive(">=", a, b);
pragma "inline" fun <(a: int(64), b: int(64)) return __primitive("<", a, b);
pragma "inline" fun >(a: int(64), b: int(64)) return __primitive(">", a, b);
pragma "inline" fun !(a: int(64)) return __primitive("!", a);
pragma "inline" fun &&(a: int(64), b: int(64)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: int(64), b: int(64)) return __primitive("||", a, b);

// Primitive functions and operators on uint(8)
pragma "inline" fun =(a: uint(8), b: uint) return b:uint(8);
//pragma "inline" fun _copy(a: uint(8)) return a;
pragma "inline" fun +(a: uint(8)) return __primitive("u+", a);
pragma "inline" fun -(a: uint(8)) return __primitive("u-", a);
pragma "inline" fun +(a: uint(8), b: uint(8)) return __primitive("+", a, b);
pragma "inline" fun -(a: uint(8), b: uint(8)) return __primitive("-", a, b);
pragma "inline" fun *(a: uint(8), b: uint(8)) return __primitive("*", a, b);
pragma "inline" fun /(a: uint(8), b: uint(8)) return __primitive("/", a, b);
pragma "inline" fun **(a: uint(8), b: uint(8)) return __primitive("**", a, b);
pragma "inline" fun %(a: uint(8), b: uint(8)) return __primitive("%", a, b);
pragma "inline" fun ~(a: uint(8)) return __primitive("u~", a);
pragma "inline" fun &(a: uint(8), b: uint(8)) return __primitive("&", a, b);
pragma "inline" fun |(a: uint(8), b: uint(8)) return __primitive("|", a, b);
pragma "inline" fun ^(a: uint(8), b: uint(8)) return __primitive("^", a, b);
pragma "inline" fun <<(a: uint(8), b: uint(8)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: uint(8), b: uint(8)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: uint(8), b: uint(8)) return __primitive("==", a, b);
pragma "inline" fun !=(a: uint(8), b: uint(8)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: uint(8), b: uint(8)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: uint(8), b: uint(8)) return __primitive(">=", a, b);
pragma "inline" fun <(a: uint(8), b: uint(8)) return __primitive("<", a, b);
pragma "inline" fun >(a: uint(8), b: uint(8)) return __primitive(">", a, b);
pragma "inline" fun !(a: uint(8)) return __primitive("!", a);
pragma "inline" fun &&(a: uint(8), b: uint(8)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: uint(8), b: uint(8)) return __primitive("||", a, b);

// Primitive functions and operators on uint(16)
pragma "inline" fun =(a: uint(16), b: uint) return b:uint(16);
//pragma "inline" fun _copy(a: uint(16)) return a;
pragma "inline" fun +(a: uint(16)) return __primitive("u+", a);
pragma "inline" fun -(a: uint(16)) return __primitive("u-", a);
pragma "inline" fun +(a: uint(16), b: uint(16)) return __primitive("+", a, b);
pragma "inline" fun -(a: uint(16), b: uint(16)) return __primitive("-", a, b);
pragma "inline" fun *(a: uint(16), b: uint(16)) return __primitive("*", a, b);
pragma "inline" fun /(a: uint(16), b: uint(16)) return __primitive("/", a, b);
pragma "inline" fun **(a: uint(16), b: uint(16)) return __primitive("**", a, b);
pragma "inline" fun %(a: uint(16), b: uint(16)) return __primitive("%", a, b);
pragma "inline" fun ~(a: uint(16)) return __primitive("u~", a);
pragma "inline" fun &(a: uint(16), b: uint(16)) return __primitive("&", a, b);
pragma "inline" fun |(a: uint(16), b: uint(16)) return __primitive("|", a, b);
pragma "inline" fun ^(a: uint(16), b: uint(16)) return __primitive("^", a, b);
pragma "inline" fun <<(a: uint(16), b: uint(16)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: uint(16), b: uint(16)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: uint(16), b: uint(16)) return __primitive("==", a, b);
pragma "inline" fun !=(a: uint(16), b: uint(16)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: uint(16), b: uint(16)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: uint(16), b: uint(16)) return __primitive(">=", a, b);
pragma "inline" fun <(a: uint(16), b: uint(16)) return __primitive("<", a, b);
pragma "inline" fun >(a: uint(16), b: uint(16)) return __primitive(">", a, b);
pragma "inline" fun !(a: uint(16)) return __primitive("!", a);
pragma "inline" fun &&(a: uint(16), b: uint(16)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: uint(16), b: uint(16)) return __primitive("||", a, b);

// Primitive functions and operators on uint(32)
pragma "inline" fun =(a: uint(32), b: uint) return b:uint(32);
//pragma "inline" fun _copy(a: uint(32)) return a;
pragma "inline" fun +(a: uint(32)) return __primitive("u+", a);
pragma "inline" fun -(a: uint(32)) return __primitive("u-", a);
pragma "inline" fun +(a: uint(32), b: uint(32)) return __primitive("+", a, b);
pragma "inline" fun -(a: uint(32), b: uint(32)) return __primitive("-", a, b);
pragma "inline" fun *(a: uint(32), b: uint(32)) return __primitive("*", a, b);
pragma "inline" fun /(a: uint(32), b: uint(32)) return __primitive("/", a, b);
pragma "inline" fun **(a: uint(32), b: uint(32)) return __primitive("**", a, b);
pragma "inline" fun %(a: uint(32), b: uint(32)) return __primitive("%", a, b);
pragma "inline" fun ~(a: uint(32)) return __primitive("u~", a);
pragma "inline" fun &(a: uint(32), b: uint(32)) return __primitive("&", a, b);
pragma "inline" fun |(a: uint(32), b: uint(32)) return __primitive("|", a, b);
pragma "inline" fun ^(a: uint(32), b: uint(32)) return __primitive("^", a, b);
pragma "inline" fun <<(a: uint(32), b: uint(32)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: uint(32), b: uint(32)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: uint(32), b: uint(32)) return __primitive("==", a, b);
pragma "inline" fun !=(a: uint(32), b: uint(32)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: uint(32), b: uint(32)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: uint(32), b: uint(32)) return __primitive(">=", a, b);
pragma "inline" fun <(a: uint(32), b: uint(32)) return __primitive("<", a, b);
pragma "inline" fun >(a: uint(32), b: uint(32)) return __primitive(">", a, b);
pragma "inline" fun !(a: uint(32)) return __primitive("!", a);
pragma "inline" fun &&(a: uint(32), b: uint(32)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: uint(32), b: uint(32)) return __primitive("||", a, b);

// Primitive functions and operators on uint(64)
pragma "inline" fun =(a: uint(64), b: uint(64)) return b;
//pragma "inline" fun _copy(a: uint(64)) return a;
pragma "inline" fun +(a: uint(64)) return __primitive("u+", a);
pragma "inline" fun -(a: uint(64)) return __primitive("u-", a);
pragma "inline" fun +(a: uint(64), b: uint(64)) return __primitive("+", a, b);
pragma "inline" fun -(a: uint(64), b: uint(64)) return __primitive("-", a, b);
pragma "inline" fun *(a: uint(64), b: uint(64)) return __primitive("*", a, b);
pragma "inline" fun /(a: uint(64), b: uint(64)) return __primitive("/", a, b);
pragma "inline" fun **(a: uint(64), b: uint(64)) return __primitive("**", a, b);
pragma "inline" fun %(a: uint(64), b: uint(64)) return __primitive("%", a, b);
pragma "inline" fun ~(a: uint(64)) return __primitive("u~", a);
pragma "inline" fun &(a: uint(64), b: uint(64)) return __primitive("&", a, b);
pragma "inline" fun |(a: uint(64), b: uint(64)) return __primitive("|", a, b);
pragma "inline" fun ^(a: uint(64), b: uint(64)) return __primitive("^", a, b);
pragma "inline" fun <<(a: uint(64), b: uint(64)) return __primitive("<<", a, b);
pragma "inline" fun >>(a: uint(64), b: uint(64)) return __primitive(">>", a, b);
pragma "inline" fun ==(a: uint(64), b: uint(64)) return __primitive("==", a, b);
pragma "inline" fun !=(a: uint(64), b: uint(64)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: uint(64), b: uint(64)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: uint(64), b: uint(64)) return __primitive(">=", a, b);
pragma "inline" fun <(a: uint(64), b: uint(64)) return __primitive("<", a, b);
pragma "inline" fun >(a: uint(64), b: uint(64)) return __primitive(">", a, b);
pragma "inline" fun !(a: uint(64)) return __primitive("!", a);
pragma "inline" fun &&(a: uint(64), b: uint(64)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: uint(64), b: uint(64)) return __primitive("||", a, b);

// Primitive functions and operators on float(32)
pragma "inline" fun =(a: float(32), b: float) return b:float(32);
//pragma "inline" fun _copy(a: float(32)) return a;
pragma "inline" fun +(a: float(32)) return __primitive("u+", a);
pragma "inline" fun -(a: float(32)) return __primitive("u-", a);
pragma "inline" fun +(a: float(32), b: float(32)) return __primitive("+", a, b);
pragma "inline" fun -(a: float(32), b: float(32)) return __primitive("-", a, b);
pragma "inline" fun *(a: float(32), b: float(32)) return __primitive("*", a, b);
pragma "inline" fun /(a: float(32), b: float(32)) return __primitive("/", a, b);
pragma "inline" fun **(a: float(32), b: float(32)) return __primitive("**", a, b);
pragma "inline" fun ==(a: float(32), b: float(32)) return __primitive("==", a, b);
pragma "inline" fun !=(a: float(32), b: float(32)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: float(32), b: float(32)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: float(32), b: float(32)) return __primitive(">=", a, b);
pragma "inline" fun <(a: float(32), b: float(32)) return __primitive("<", a, b);
pragma "inline" fun >(a: float(32), b: float(32)) return __primitive(">", a, b);
pragma "inline" fun !(a: float(32)) return __primitive("!", a);
pragma "inline" fun &&(a: float(32), b: float(32)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: float(32), b: float(32)) return __primitive("||", a, b);

// Primitive functions and operators on float(64)
pragma "inline" fun =(a: float(64), b: float(64)) return b;
//pragma "inline" fun _copy(a: float(64)) return a;
pragma "inline" fun +(a: float(64)) return __primitive("u+", a);
pragma "inline" fun -(a: float(64)) return __primitive("u-", a);
pragma "inline" fun +(a: float(64), b: float(64)) return __primitive("+", a, b);
pragma "inline" fun -(a: float(64), b: float(64)) return __primitive("-", a, b);
pragma "inline" fun *(a: float(64), b: float(64)) return __primitive("*", a, b);
pragma "inline" fun /(a: float(64), b: float(64)) return __primitive("/", a, b);
pragma "inline" fun **(a: float(64), b: float(64)) return __primitive("**", a, b);
pragma "inline" fun ==(a: float(64), b: float(64)) return __primitive("==", a, b);
pragma "inline" fun !=(a: float(64), b: float(64)) return __primitive("!=", a, b);
pragma "inline" fun <=(a: float(64), b: float(64)) return __primitive("<=", a, b);
pragma "inline" fun >=(a: float(64), b: float(64)) return __primitive(">=", a, b);
pragma "inline" fun <(a: float(64), b: float(64)) return __primitive("<", a, b);
pragma "inline" fun >(a: float(64), b: float(64)) return __primitive(">", a, b);
pragma "inline" fun !(a: float(64)) return __primitive("!", a);
pragma "inline" fun &&(a: float(64), b: float(64)) return __primitive("&&", a, b);
pragma "inline" fun ||(a: float(64), b: float(64)) return __primitive("||", a, b);

// Primitive functions and operators on string
pragma "inline" fun =(a: string, b: string) return __primitive("string_copy", b);
//pragma "inline" fun _copy(x : string) {
//  var y : string;
//  y = x;
//  return y;
//}
pragma "inline" fun ==(a: string, b: string) return __primitive("string_equal", a, b);
pragma "inline" fun !=(a: string, b: string) return !(a == b);
pragma "inline" fun +(a: string, b: string) return __primitive("string_concat", a, b);
pragma "inline" fun length(a: string) return __primitive("string_length", a);
pragma "inline" fun string.this(i: int) return __primitive("string_index", this, i);

pragma "inline" fun _string_contains(a: string, b: string)
  return __primitive("string_contains", a, b);

pragma "inline" fun _tostring(x : bool, format : string)
  return __primitive("to_string", format, x);

pragma "inline" fun _tostring(x : int, format : string)
  return __primitive("to_string", format, x);

pragma "inline" fun _tostring(x : float, format : string)
  return __primitive("to_string", format, x);

pragma "inline" fun ascii(a: string) return __primitive("ascii", a);

//
// Primitive funs for objects
//
pragma "inline" fun ==(a: object, b: object) return __primitive("ptr_eq", a, b);
pragma "inline" fun !=(a: object, b: object) return __primitive("ptr_neq", a, b);
pragma "inline" fun =(a, b:object) return b:a.type;
pragma "inline" fun _copy(a) return a;



//
// _init
//
pragma "inline" fun _init(x : bool) return false;
pragma "inline" fun _init(x : int) return 0;
pragma "inline" fun _init(x : float) return 0.0;
pragma "inline" fun _init(x : string) return "";
pragma "inline" fun _init(x) return nil:x;


//
// More primitive funs
//
pragma "no codegen" pragma "rename _chpl_exit" fun exit(status : int) {
  __primitive("exit");       
}

pragma "no codegen" pragma "rename fflush" fun fflush(fp: _file) : int {
  return __primitive("fflush", fp);
}

fun init_elts(x, s, e) {
  for i in 1..s {
    var y : e.type;
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
    __primitive("array_init", this, size);
    init_elts(this, size, _element);
  }
  pragma "inline" fun this(i : int) {
    return __primitive("array_index", this, i);
  }
  pragma "inline" fun =this(i : int, val : elt_type) {
    __primitive("array_set", this, i, val);
  }
}


// synch variable support
pragma "sync var"
record _syncvar {
  type t;
  var  value: t;             // actual data
  var  is_full: bool;
  var  lock: _mutex_p;       // need to acquire before accessing this record
  var  cv_empty: _condvar_p; // wait for empty, signal this when empty
  var  cv_full: _condvar_p;  // wait for full, signal this when full
}

// The operations are:
//  = (i.e., write_wait_empty_leave_full)
//  write_leave full
//  read_wait_full_leave_empty
//  read_wait_full_leave_full
//  read

// wait for empty, leaves full. This is the default write on sync vars.
fun =( sv:_syncvar, value:sv.t) {
  __primitive( "syncvar_lock", sv);
  if (sv.is_full) {
    __primitive( "syncvar_wait_empty", sv);
  }
  sv.value = value;
  sv.is_full = true;
  __primitive( "syncvar_signal_full", sv);
  __primitive( "syncvar_unlock", sv);
  return sv;
}

// no wait, leave full. This is the default read on sync vars.
fun write_leave_full( sv:_syncvar) {
  var ret: sv.t;
  __primitive( "syncvar_lock", sv);
  ret = sv.value;
  sv.is_full = true;
  __primitive( "syncvar_signal_full", sv);
  __primitive( "syncvar_unlock", sv);
  return ret;
}

// wait for full, leave empty. This is the default read on sync vars.
fun read_wait_full_leave_empty( sv:_syncvar) {
  var ret: sv.t;
  __primitive( "syncvar_lock", sv);
  if (!sv.is_full) {
    __primitive( "syncvar_wait_full", sv);
  }
  ret = sv.value;
  sv.is_full = false;
  __primitive( "syncvar_signal_empty", sv);
  __primitive( "syncvar_unlock", sv);
  return ret;
}

// wait for full, leave full.
fun read_wait_full_leave_full( sv:_syncvar) {
  var ret: sv.t;
  __primitive( "syncvar_lock", sv);
  if (!sv.is_full) {
    __primitive( "syncvar_wait_full", sv);
  }
  ret = sv.value;
  __primitive( "syncvar_signal_full", sv);
  __primitive( "syncvar_unlock", sv);
  return ret;
}

// read value.  No state change or signals
fun read( sv:_syncvar) {
  var ret: sv.t;
  __primitive( "syncvar_lock", sv);
  ret = sv.value;
  __primitive( "syncvar_unlock", sv);
  return ret;
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
