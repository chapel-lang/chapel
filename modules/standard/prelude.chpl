pragma "no codegen";

class object { }
class value { }

function typeof(x) { }

function sizeof(x) { return __primitive("sizeof", integer); }

// dynamic data block class
pragma "data class"
class _ddata {
  type elt_type;
  var size : integer;
  pragma "data block" var _elt : elt_type;
  pragma "rename _data_get"
  function this(i : integer) : elt_type {
    return _elt;
  }
  pragma "rename _data_set"
  function =this(i : integer, val : elt_type) {
    _elt = val;
  }
}

-- I/O

class CFILEPTR { 
  var FILEptr : integer;
}

const _NULLCFILEPTR   : CFILEPTR = CFILEPTR(-1);
const _STDINCFILEPTR  : CFILEPTR = CFILEPTR(0);
const _STDOUTCFILEPTR : CFILEPTR = CFILEPTR(1);
const _STDERRCFILEPTR : CFILEPTR = CFILEPTR(2);

pragma "rename fopen" function _fopen(filename, mode : string) : CFILEPTR {
  return CFILEPTR(__primitive("fopen", filename, mode));
}

pragma "rename fclose" function _fclose(fp : CFILEPTR) : integer {
  return __primitive("fclose", fp.FILEptr);
}

const errno: integer;
function strerror(errno: integer) : string {
  return __primitive("strerror", errno);
}

pragma "rename fprintf"
function fprintf(fp: CFILEPTR, fmt: string, val) : integer {
  return __primitive("fprintf", fp.FILEptr, fmt, val);
}

pragma "rename fscanf"
function fscanf(fp: CFILEPTR, fmt: string, inout val) : integer {
  return __primitive("fscanf", fp.FILEptr, string, val);
}

function _chpl_fwrite_float_help(fp: CFILEPTR, val: float) : void {
  __primitive("fprintf", fp.FILEptr, "%g", val);
}

function _chpl_fread_string_help(f: CFILEPTR, inout val: string) : void {
  __primitive("fscanf", "%g", val);
}

function _readLitChar(f: CFILEPTR, val: string, ignoreWhiteSpace: integer) : integer {
  return __primitive("fscanf", "%s", val);
}

pragma "rename _chpl_tostring_boolean"
function _tostring(x : boolean, format : string) : string {
  return __primitive("to_string", format, x);
}

pragma "rename _chpl_tostring_integer"
function _tostring(x : integer, format : string) : string {
  return __primitive("to_string", format, x);
}

pragma "rename _chpl_tostring_float"
function _tostring(x : float, format : string) : string {
         return __primitive("to_string", format, x);
}

pragma "rename _chpl_tostring_complex"
function _tostring(x : complex, format : string) : string {
         return __primitive("to_string", format, x);
}


-- intrinsic type values

const false: boolean = 0:boolean;
const true: boolean = 1:boolean;


-- math

function sqrt(x: float): float {}
function abs(x): float {}  -- BLC: should be ": t"  SJD: redo with primitive?
function ceil(x: float): float {}
function min(x, y) {
  if (x < y) {
    return x;
  } else {
    return y;
  }
}
function max(x, y) {
  if (x > y) {
    return x;
  } else {
    return y;
  }
}



-- boundary classes/functions

function wrap() {}


-- reductions

class reduction {
}

class sum {
}

class max {
}

class min {
}

class maxloc {
}

class minloc {
}


-- timers

class timer {
  function start();
  function stop();
  function read(): float;
}

-- exits

pragma "rename _chpl_exit" function exit(status : integer) {
  __primitive("exit");       
}

function halt() {
  __primitive("halt");
}

function assert() {
  __primitive("assert");
}


-- memory tests (These tests will be moved to a module, once we have modules.)

function _chpl_memtest_printMemTable();
function _chpl_memtest_printMemStat();
function _chpl_memtest_resetMemStat();
function _chpl_memtest_allocAndFree();
function _chpl_memtest_freedMalloc();
function _chpl_memtest_freedWithoutMalloc();
function _chpl_memtest_reallocWithoutMalloc();
function _chpl_memtest_reallocZeroSize();
function _chpl_memtest_mallocOutOfMemory();
function _chpl_memtest_reallocOutOfMemory();

function startTrackingMem();

function _complex_read_hack(inout x) : integer {
  return __primitive("fscanf", "%g%g", x);
}

function _complex_tostring_hack(x, format : string) : string {
  return __primitive("to_string", format, x);
}




const numLocales: integer = 1;


-- FROM INTERNAL PRELUDE

record _methodTokenType {
  var _methodTokenTypeFieldDummy : integer;
}

var _methodToken : _methodTokenType;

record _setterTokenType {
  var _setterTokenTypeFieldDummy : integer;
}

var _setterToken : _setterTokenType;

class _seq { -- this is no longer necessary
}

record Array {
  var this_is_an_array : float;
  function this(i) {
    return __primitive("array_index", this, i);
  }
  function this(i, j) {
    return __primitive("array_index", this, i, j);
  }
  function this(i, j, k) {
    return __primitive("array_index", this, i, j, k);
  }
  function =this(i, x) {
    return __primitive("array_set", this, i, x);
  }
  function =this(i, j, x) {
    return __primitive("array_set", this, i, j, x);
  }
  function =this(i, j, k, x) {
    return __primitive("array_set", this, i, j, k, x);
  }
}

class Domain {
}

record Tuple {
}

pragma "builtin" function +(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function -(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function *(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function /(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function &(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function |(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function ^(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function ==(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function !=(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function <=(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function >=(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function <(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function >(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function and(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function or(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function **(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }

pragma "builtin" function -(a : numeric) { return __primitive("u-", a); }
pragma "builtin" function +(a : numeric) { return __primitive("u+", a); }
pragma "builtin" function ~(a : numeric) { return __primitive("u~", a); }
pragma "builtin" function not(a : numeric) { return __primitive("not", a); }

pragma "builtin" function +(a : numeric, b : numeric) { return __primitive("+", a, b); }
pragma "builtin" function -(a : numeric, b : numeric) { return __primitive("-", a, b); }
pragma "builtin" function *(a : numeric, b : numeric) { return __primitive("*", a, b); }
pragma "builtin" function /(a : numeric, b : numeric) { return __primitive("/", a, b); }
pragma "builtin" function mod(a : numeric, b : numeric) { return __primitive("%", a, b); }
pragma "builtin" function ==(a : numeric, b : numeric) { return __primitive("==", a, b); } 
pragma "builtin" function !=(a : numeric, b : numeric) { return __primitive("!=", a, b); }
pragma "builtin" function <=(a : numeric, b : numeric) { return __primitive("<=", a, b); }
pragma "builtin" function >=(a : numeric, b : numeric) { return __primitive(">=", a, b); }
pragma "builtin" function <(a : numeric, b : numeric) { return __primitive("<", a, b); }
pragma "builtin" function >(a : numeric, b : numeric) { return __primitive(">", a, b); }
pragma "builtin" function &(a : numeric, b : numeric) { return __primitive("&", a, b); }
pragma "builtin" function |(a : numeric, b : numeric) { return __primitive("|", a, b); }
pragma "builtin" function ^(a : numeric, b : numeric) { return __primitive("^", a, b); }
pragma "builtin" function and(a : numeric, b : numeric) { return __primitive("&&", a, b); }
pragma "builtin" function or(a : numeric, b : numeric) { return __primitive("||", a, b); }
pragma "builtin" function **(a : numeric, b : numeric) { return __primitive("**", a, b); }
pragma "builtin" function ==(a : object, b : object) { return __primitive("ptr_eq", a, b); }
pragma "builtin" function !=(a : object, b : object) { return __primitive("ptr_neq", a, b); }

pragma "split unique"
pragma "builtin" function =(a : numeric, b : any) { 
  return __primitive("cast", a, b);
}

pragma "rename _copy_string" function =(a : string, b : any) : string { 
  return __primitive("copy_string", string);
}

pragma "split unique"
pragma "builtin" function =(a : any, b : any) { 
  return b; 
}

pragma "rename _chpl_string_index"
function string.this(i : integer) {
  return __primitive("string_index", this, i);
}

pragma "rename _chpl_string_select"
function _chpl_string_select(s : string, low : integer, high : integer) {
  return __primitive("string_select", s, low, high);
}

pragma "rename _chpl_string_strided_select"
function _chpl_string_strided_select(s : string, low : integer, high : integer, stride : integer) {
  return __primitive("string_strided_select", s, low, high, stride);
}

pragma "rename _chpl_string_concat"
function +(a : string, b : string) : string {
  return __primitive("string_concat", a, b);
}

pragma "rename _chpl_string_equal"
function ==(a : string, b : string) : boolean { 
  return __primitive("string_equal", a, b);
}

pragma "rename _chpl_string_notequal"
function !=(a : string, b : string) : boolean { 
  return not __primitive("string_equal", a, b);
}

pragma "rename _chpl_string_length"
function length(a : string) : integer {
  return __primitive("string_length", a);
}

pragma "split unique"
pragma "keep types"
pragma "rename _chpl_alloc"
function _chpl_alloc(t, description:string) { return __primitive("chpl_alloc", t); }

function _init_string() { return ""; }

function _INIT_CONFIG(inout v, v_type, chapel_name, module_name);
function _INIT_ARRAY(rank, arr, dom, elt_type);
function _INIT_DOMAIN_DIM(dom, dim, lo, hi, str);

function _ArrayWriteStopgap(x) { }
function _EnumReadStopgap(x) { }
function _DomainWriteStopgap(x) { }

-- END FROM INTERNAL PRELUDE
