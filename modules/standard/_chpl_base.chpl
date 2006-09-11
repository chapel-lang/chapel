// _chpl_base module

// Primitive functions and operators on bool
pragma "inline" def =(a: bool, b: bool) return b;
pragma "inline" def ~(a: bool) return __primitive("u~", a);
pragma "inline" def &(a: bool, b: bool) return __primitive("&", a, b);
pragma "inline" def |(a: bool, b: bool) return __primitive("|", a, b);
pragma "inline" def ^(a: bool, b: bool) return __primitive("^", a, b);
pragma "inline" def ==(a: bool, b: bool) return __primitive("==", a, b);
pragma "inline" def !=(a: bool, b: bool) return __primitive("!=", a, b);
pragma "inline" def <=(a: bool, b: bool) return __primitive("<=", a, b);
pragma "inline" def >=(a: bool, b: bool) return __primitive(">=", a, b);
pragma "inline" def <(a: bool, b: bool) return __primitive("<", a, b);
pragma "inline" def >(a: bool, b: bool) return __primitive(">", a, b);
pragma "inline" def !(a: bool) return __primitive("!", a);
pragma "inline" def &&(a: bool, b: bool) return __primitive("&&", a, b);
pragma "inline" def ||(a: bool, b: bool) return __primitive("||", a, b);

// Primitive functions and operators on ints
pragma "inline" def =(a: int(?w), b: int(w)) return b;
pragma "inline" def +(a: int(?w)) return __primitive("u+", a);
pragma "inline" def -(a: int(?w)) return __primitive("u-", a);
pragma "inline" def +(a: int(?w), b: int(w)) return __primitive("+", a, b);
pragma "inline" def -(a: int(?w), b: int(w)) return __primitive("-", a, b);
pragma "inline" def *(a: int(?w), b: int(w)) return __primitive("*", a, b);
pragma "inline" def /(a: int(?w), b: int(w)) return __primitive("/", a, b);
pragma "inline" def **(a: int(?w), b: int(w)) return __primitive("**", a, b);
pragma "inline" def %(a: int(?w), b: int(w)) return __primitive("%", a, b);
pragma "inline" def ~(a: int(?w)) return __primitive("u~", a);
pragma "inline" def &(a: int(?w), b: int(w)) return __primitive("&", a, b);
pragma "inline" def |(a: int(?w), b: int(w)) return __primitive("|", a, b);
pragma "inline" def ^(a: int(?w), b: int(w)) return __primitive("^", a, b);
pragma "inline" def <<(a: int(?w), b: int(w)) return __primitive("<<", a, b);
pragma "inline" def >>(a: int(?w), b: int(w)) return __primitive(">>", a, b);
pragma "inline" def ==(a: int(?w), b: int(w)) return __primitive("==", a, b);
pragma "inline" def !=(a: int(?w), b: int(w)) return __primitive("!=", a, b);
pragma "inline" def <=(a: int(?w), b: int(w)) return __primitive("<=", a, b);
pragma "inline" def >=(a: int(?w), b: int(w)) return __primitive(">=", a, b);
pragma "inline" def <(a: int(?w), b: int(w)) return __primitive("<", a, b);
pragma "inline" def >(a: int(?w), b: int(w)) return __primitive(">", a, b);
pragma "inline" def !(a: int(?w)) return __primitive("!", a);
pragma "inline" def &&(a: int(?w), b: int(w)) return __primitive("&&", a, b);
pragma "inline" def ||(a: int(?w), b: int(w)) return __primitive("||", a, b);

// Primitive functions and operators on uints
pragma "inline" def =(a: uint(?w), b: uint(w)) return b;
pragma "inline" def +(a: uint(?w)) return __primitive("u+", a);
pragma "inline" def -(a: uint(?w)) return __primitive("u-", a);
pragma "inline" def +(a: uint(?w), b: uint(w)) return __primitive("+", a, b);
pragma "inline" def -(a: uint(?w), b: uint(w)) return __primitive("-", a, b);
pragma "inline" def *(a: uint(?w), b: uint(w)) return __primitive("*", a, b);
pragma "inline" def /(a: uint(?w), b: uint(w)) return __primitive("/", a, b);
pragma "inline" def **(a: uint(?w), b: uint(w)) return __primitive("**", a, b);
pragma "inline" def %(a: uint(?w), b: uint(w)) return __primitive("%", a, b);
pragma "inline" def ~(a: uint(?w)) return __primitive("u~", a);
pragma "inline" def &(a: uint(?w), b: uint(w)) return __primitive("&", a, b);
pragma "inline" def |(a: uint(?w), b: uint(w)) return __primitive("|", a, b);
pragma "inline" def ^(a: uint(?w), b: uint(w)) return __primitive("^", a, b);
pragma "inline" def <<(a: uint(?w), b: uint(w)) return __primitive("<<", a, b);
pragma "inline" def >>(a: uint(?w), b: uint(w)) return __primitive(">>", a, b);
pragma "inline" def ==(a: uint(?w), b: uint(w)) return __primitive("==", a, b);
pragma "inline" def !=(a: uint(?w), b: uint(w)) return __primitive("!=", a, b);
pragma "inline" def <=(a: uint(?w), b: uint(w)) return __primitive("<=", a, b);
pragma "inline" def >=(a: uint(?w), b: uint(w)) return __primitive(">=", a, b);
pragma "inline" def <(a: uint(?w), b: uint(w)) return __primitive("<", a, b);
pragma "inline" def >(a: uint(?w), b: uint(w)) return __primitive(">", a, b);
pragma "inline" def !(a: uint(?w)) return __primitive("!", a);
pragma "inline" def &&(a: uint(?w), b: uint(w)) return __primitive("&&", a, b);
pragma "inline" def ||(a: uint(?w), b: uint(w)) return __primitive("||", a, b);

// Primitive functions and operators on floats
pragma "inline" def =(a: float(?w), b: float(w)) return b;
pragma "inline" def +(a: float(?w)) return __primitive("u+", a);
pragma "inline" def -(a: float(?w)) return __primitive("u-", a);
pragma "inline" def +(a: float(?w), b: float(w)) return __primitive("+", a, b);
pragma "inline" def -(a: float(?w), b: float(w)) return __primitive("-", a, b);
pragma "inline" def *(a: float(?w), b: float(w)) return __primitive("*", a, b);
pragma "inline" def /(a: float(?w), b: float(w)) return __primitive("/", a, b);
pragma "inline" def **(a: float(?w), b: float(w)) return __primitive("**", a, b);
pragma "inline" def ==(a: float(?w), b: float(w)) return __primitive("==", a, b);
pragma "inline" def !=(a: float(?w), b: float(w)) return __primitive("!=", a, b);
pragma "inline" def <=(a: float(?w), b: float(w)) return __primitive("<=", a, b);
pragma "inline" def >=(a: float(?w), b: float(w)) return __primitive(">=", a, b);
pragma "inline" def <(a: float(?w), b: float(w)) return __primitive("<", a, b);
pragma "inline" def >(a: float(?w), b: float(w)) return __primitive(">", a, b);
pragma "inline" def !(a: float(?w)) return __primitive("!", a);
pragma "inline" def &&(a: float(?w), b: float(w)) return __primitive("&&", a, b);
pragma "inline" def ||(a: float(?w), b: float(w)) return __primitive("||", a, b);

// Primitive functions and operators on string
pragma "inline" def =(a: string, b: string) return __primitive("string_copy", b);
pragma "inline" def ==(a: string, b: string) return __primitive("string_equal", a, b);
pragma "inline" def !=(a: string, b: string) return !(a == b);
pragma "inline" def +(a: string, b: string) return __primitive("string_concat", a, b);
pragma "inline" def length(a: string) return __primitive("string_length", a);
pragma "inline" def string.this(i: int) return __primitive("string_index", this, i);

pragma "inline" def _string_contains(a: string, b: string)
  return __primitive("string_contains", a, b);

pragma "inline" def _tostring(x : bool, format : string)
  return __primitive("to_string", format, x);

pragma "inline" def _tostring(x : int, format : string)
  return __primitive("to_string", format, x);

pragma "inline" def _tostring(x : float, format : string)
  return __primitive("to_string", format, x);

pragma "inline" def ascii(a: string) return __primitive("ascii", a);

//
// Primitive funs for objects
//
pragma "inline" def ==(a: object, b: object) return __primitive("ptr_eq", a, b);
pragma "inline" def !=(a: object, b: object) return __primitive("ptr_neq", a, b);
pragma "inline" def =(a, b) return b;
pragma "inline" def _copy(a) return a;



//
// _init
//
pragma "inline" def _init(x : bool) return false;
pragma "inline" def _init(x : int(?w)) return 0:int(w);
pragma "inline" def _init(x : uint(?w)) return 0:uint(w);
pragma "inline" def _init(x : float(?w)) return 0:float(w);
pragma "inline" def _init(x : string) return "";
pragma "inline" def _init(x) return nil:x;


// identity functions (for reduction operations)
pragma "inline" def _prod_id( type t) return __primitive( "_prod_id", t);
pragma "inline" def _land_id( type t) return __primitive( "_land_id", t);
pragma "inline" def _lor_id( type t) return __primitive( "_lor_id", t);
pragma "inline" def _lxor_id( type t) return __primitive( "_lxor_id", t);
pragma "inline" def _band_id( type t) return __primitive( "_band_id", t);
pragma "inline" def _bor_id( type t) return __primitive( "_bor_id", t);
pragma "inline" def _bxor_id( type t) return __primitive( "_bxor_id", t);


// predefined functions
pragma "inline" def min(type t) return __primitive( "_min", t);
pragma "inline" def max(type t) return __primitive( "_max", t);

pragma "inline" def min(x, y) return if x < y then x else y;
pragma "inline" def max(x, y) return if x > y then x else y;

pragma "inline" def min(x, y, z...?k) return min(min(x, y), (...z));
pragma "inline" def max(x, y, z...?k) return max(max(x, y), (...z));

//
// More primitive funs
//

pragma "inline" def sleep( t: int) return __primitive( "sleep", t);

pragma "no codegen" pragma "rename _chpl_exit" def exit(status : int) {
  __primitive("exit");       
}

pragma "no codegen" pragma "rename fflush" def fflush(fp: _file) : int {
  return __primitive("fflush", fp);
}

def init_elts(x, s, e) {
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
  pragma "inline" def init() {
    __primitive("array_init", this, elt_type, size);
    init_elts(this, size, _element);
  }
  pragma "inline" def this(i : int) {
    return __primitive("array_get", this, i);
  }
  pragma "inline" def =this(i : int, val : elt_type) {
    __primitive("array_set", this, i, val);
  }
}


// synch variable support
pragma "inline" def _init( cv: _mutex_p) return __primitive( "mutex_new");
pragma "inline" def =( a: _mutex_p, b: _mutex_p) return b;
pragma "inline" def _init( cv: _condvar_p) return __primitive( "condvar_new");
pragma "inline" def =( a: _condvar_p, b: _condvar_p) return b;

pragma "sync var"
pragma "no default functions"
class _syncvar {
  type t;
  var  value: t;             // actual data
  var  is_full: bool;
  var  lock: _mutex_p;       // need to acquire before accessing this record
  var  cv_empty: _condvar_p; // wait for empty, signal this when empty
  var  cv_full: _condvar_p;  // wait for full, signal this when full

  def initialize() {
    is_full = false; 
    lock = __primitive( "mutex_new");
    cv_empty = __primitive( "condvar_new");
    cv_full = __primitive( "condvar_new");
  }
}

def _copy( sv:_syncvar) {
  return readFE(sv);
}

def _init( sv:_syncvar) {
  return _syncvar( sv.value.type); 
}

// The operations are:
//  = (i.e., write_wait_empty_leave_full)
//  writeFE - wait full, leave empty
//  writeXF - no wait, leave full
//  writeXE - no wait, leave empty
//  readFE - wait full, leave empty
//  readFF - wait full, leave full
//  readXF - no wait, leave full
//  readXX - no wait, leave F/E unchanged

// wait for empty, leaves full. This is the default write on sync vars.
def =( sv:_syncvar, value:sv.t) {
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

// wait full, leave empty.
def writeFE( sv:_syncvar, value:sv.t) {
  __primitive( "syncvar_lock", sv);
  if (!sv.is_full) {
    __primitive( "syncvar_wait_full", sv);
  }
  sv.value = value;
  sv.is_full = false;
  __primitive( "syncvar_signal_empty", sv);
  __primitive( "syncvar_unlock", sv);
  return sv;
}

// no wait, leave full.
def writeXF( sv:_syncvar, value:sv.t) {
  __primitive( "syncvar_lock", sv);
  sv.value = value;
  sv.is_full = true;
  __primitive( "syncvar_signal_full", sv);
  __primitive( "syncvar_unlock", sv);
}

// no wait, leave empty. One use of this is with using sync var as a lock.
def writeXE( sv:_syncvar, value:sv.t) {
  __primitive( "syncvar_lock", sv);
  sv.value = value;
  sv.is_full = false;
  __primitive( "syncvar_signal_full", sv);
  __primitive( "syncvar_unlock", sv);
}

// wait for full, leave empty. This is the default read on sync vars.
def readFE( sv:_syncvar) {
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
def readFF( sv:_syncvar) {
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

// no wait, leave full.
def readXF( sv:_syncvar) {
  var ret: sv.t;
  __primitive( "syncvar_lock", sv);
  ret = sv.value;
  __primitive( "syncvar_signal_full", sv);
  __primitive( "syncvar_unlock", sv);
  return ret;
}

// read value.  No state change or signals.
def readXX( sv:_syncvar) {
  var ret: sv.t;
  __primitive( "syncvar_lock", sv);
  ret = sv.value;
  __primitive( "syncvar_unlock", sv);
  return ret;
}


def isFull( sv:_syncvar) {
  var isfull: bool;
  __primitive( "syncvar_lock", sv);
  isfull = sv.is_full;
  __primitive( "syncvar_unlock", sv);
  return isfull;
}


// single variable support
pragma "single var" 
pragma "no default functions"
class _singlevar {
  type t;
  var  value: t;             // actual data
  var  is_full: bool;
  var  lock: _mutex_p;       // need to acquire before accessing this record
  var  cv_full: _condvar_p;  // wait for full

  def initialize() {
    is_full = false; 
    lock = __primitive( "mutex_new");
    cv_full = __primitive( "condvar_new");
  }
}

def _copy( sv:_singlevar) {
  return readFF( sv);
}

def _init( sv:_singlevar) {
  return _singlevar( sv.value.type); 
}


// Can only write once.  Otherwise, it is an error.
def =( sv:_singlevar, value:sv.t) {
  __primitive( "singlevar_lock", sv);
  if (sv.is_full) {
    halt( "***Error: single var already defined***");
  }
  sv.value = value;
  sv.is_full = true;
  __primitive( "singlevar_signal_full", sv);
  __primitive( "singlevar_unlock", sv);
  return sv;
}


// Wait for full
def readFF( sv:_singlevar) {
  var ret: sv.t;
  __primitive( "singlevar_lock", sv);
  if (!sv.is_full) {
    __primitive( "singlevar_wait_full", sv);
  }
  ret = sv.value;
  __primitive( "singlevar_signal_full", sv); // in case others are waiting
  __primitive( "singlevar_unlock", sv);
  return ret;
}


//
// MOVE THESE, maybe to a memory module?
//
pragma "no codegen" def _chpl_memtest_printMemTable(threshold: int = 1000);
pragma "no codegen" def _chpl_memtest_printMemStat();
pragma "no codegen" def _chpl_memtest_resetMemStat();
pragma "no codegen" def _chpl_memtest_allocAndFree();
pragma "no codegen" def _chpl_memtest_freedMalloc();
pragma "no codegen" def _chpl_memtest_freedWithoutMalloc();
pragma "no codegen" def _chpl_memtest_reallocWithoutMalloc();
pragma "no codegen" def _chpl_memtest_reallocZeroSize();
pragma "no codegen" def _chpl_memtest_mallocOutOfMemory();
pragma "no codegen" def _chpl_memtest_reallocOutOfMemory();

pragma "no codegen" def startTrackingMem();
