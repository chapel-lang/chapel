pragma "no codegen";

class object { }
class value { }

function typeof(x) { }

function sizeof(x) { return __primitive("pure_return", integer); }

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
}

const _NULLCFILEPTR   : CFILEPTR = CFILEPTR();
const _STDINCFILEPTR  : CFILEPTR = CFILEPTR();
const _STDOUTCFILEPTR : CFILEPTR = CFILEPTR();
const _STDERRCFILEPTR : CFILEPTR = CFILEPTR();

pragma "rename fopen" function _fopen(filename, mode : string) : CFILEPTR {
  return CFILEPTR();
}

pragma "rename fclose" function _fclose(fp : CFILEPTR) : integer {
  return __primitive("pure_return", integer);
}

const errno: integer;
function strerror(errno: integer) : string {
  return __primitive("pure_return", string);
}

function fprintf(fp: CFILEPTR, fmt: string, val) : integer {
  return __primitive("write", val);
}

function fscanf(fp: CFILEPTR, fmt: string, inout val) : integer {
  return __primitive("read", val);
}

function _chpl_fwrite_float_help(f: CFILEPTR, val: float) : void {
         __primitive("write", val);
}

function _chpl_fread_string_help(f: CFILEPTR, inout val: string) : void {
         __primitive("read", val);
}

function _readLitChar(f: CFILEPTR, val: string, 
                      ignoreWhiteSpace: integer) : integer {
  return __primitive("read", val);
}

pragma "rename _chpl_tostring_boolean"
function _tostring(x : boolean, format : string) : string {
  return __primitive("coerce", string, x);
}

pragma "rename _chpl_tostring_integer"
function _tostring(x : integer, format : string) : string {
  return __primitive("coerce", string, x);
}

pragma "rename _chpl_tostring_float"
function _tostring(x : float, format : string) : string {
         return __primitive("coerce", string, x);
}

pragma "rename _chpl_tostring_complex"
function _tostring(x : complex, format : string) : string {
         return __primitive("coerce", string, x);
}


-- intrinsic type values

const false: boolean = 0;
const true: boolean = 1;


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

pragma "rename _chpl_exit" function exit(status : integer);

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
  return __primitive("read", x);
}

function _complex_tostring_hack(x, format : string) : string {
  return __primitive("coerce", string, x);
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
  var _tuple_dummy_field : integer;
  function this(pragma "clone_for_constants" i) { 
    return __primitive("index_object", this, i); 
  }
  function =this(pragma "clone_for_constants" i, x) {
    return __primitive("set_index_object", this, i, x);
  }
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

pragma "builtin" function -(a : numeric) { return a; } -- needs to be primitive
pragma "builtin" function +(a : numeric) { return a; } -- needs to be primitive
pragma "builtin" function ~(a : numeric) { return a; } -- needs to be primitive
pragma "builtin" function not(a : numeric) { return a; } -- needs to be primitive

pragma "builtin" function +(a : numeric, b : numeric) { return __primitive(a, "+", b); }
pragma "builtin" function -(a : numeric, b : numeric) { return __primitive(a, "-", b); }
pragma "builtin" function *(a : numeric, b : numeric) { return __primitive(a, "*", b); }
pragma "builtin" function /(a : numeric, b : numeric) { return __primitive(a, "/", b); }
pragma "builtin" function mod(a : numeric, b : numeric) { return __primitive(a, "%", b); }
pragma "builtin" function ==(a : numeric, b : numeric) { return __primitive(a, "==", b); } 
pragma "builtin" function !=(a : numeric, b : numeric) { return __primitive(a, "!=", b); }
pragma "builtin" function <=(a : numeric, b : numeric) { return __primitive(a, "<=", b); }
pragma "builtin" function >=(a : numeric, b : numeric) { return __primitive(a, ">=", b); }
pragma "builtin" function <(a : numeric, b : numeric) { return __primitive(a, "<", b); }
pragma "builtin" function >(a : numeric, b : numeric) { return __primitive(a, ">", b); }
pragma "builtin" function &(a : numeric, b : numeric) { return __primitive(a, "&", b); }
pragma "builtin" function |(a : numeric, b : numeric) { return __primitive(a, "|", b); }
pragma "builtin" function ^(a : numeric, b : numeric) { return __primitive(a, "^", b); }
pragma "builtin" function and(a : numeric, b : numeric) { return __primitive(a, "&&", b); }
pragma "builtin" function or(a : numeric, b : numeric) { return __primitive(a, "||", b); }
pragma "builtin" function **(a : numeric, b : numeric) { return __primitive(a, "**", b); }
pragma "builtin" function ==(a : object, b : object) { return __primitive("ptr_eq", a, b); }
pragma "builtin" function !=(a : object, b : object) { return __primitive("ptr_neq", a, b); }

pragma "builtin" function _move(a : any, b : any);

pragma "split unique"
pragma "builtin" function =(a : numeric, b : any) { 
  return __primitive("cast", a, b);
}

pragma "builtin" function =(a : string, b : any) { 
  return __primitive("pure_return", string);
}

pragma "split unique"
pragma "builtin" function =(a : any, b : any) { 
  return b; 
}

pragma "rename _chpl_string_index"
function string.this(i : integer) {
  return __primitive("string_op", this, i);
}

pragma "rename _chpl_string_select"
function _chpl_string_select(s : string, low : integer, high : integer) {
  return __primitive("string_op", s, low, high);
}

pragma "rename _chpl_string_strided_select"
function _chpl_string_strided_select(s : string, low : integer, high : integer, stride : integer) {
  return __primitive("string_op", s, low, high, stride);
}

pragma "rename _chpl_string_concat"
function +(a : string, b : string) : string {
  return __primitive("string_op", a, b);
}

pragma "rename _chpl_string_equal"
function ==(a : string, b : string) : boolean { 
  return __primitive("pure_return", boolean);
}

pragma "rename _chpl_string_notequal"
function !=(a : string, b : string) : boolean { 
  return __primitive("pure_return", boolean);
}

pragma "rename _chpl_string_length"
function length(a : string) : integer {
  return __primitive("coerce", integer, a);
}

pragma "split unique"
function _alloc(t) { return __primitive("chapel_defexpr", t); }

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
