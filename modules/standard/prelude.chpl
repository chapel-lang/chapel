pragma "no codegen";

class object { }
class value { }
class closure : object { }

function typeof(x) { }

function sizeof(x) { return __primitive("sizeof", integer); }

// dynamic data block class
pragma "data class"
class _ddata {
  type elt_type;
  var size : integer;
  var _element : elt_type;
  pragma "rename _noop"
  function init() {
    return __primitive("array_init", this, size, _element) ;
  }
  pragma "rename _data_get"
  function this(i : integer) : elt_type {
    return __primitive("array_index", this, i);
  }
  pragma "rename _data_set"
  function =this(i : integer, val : elt_type) {
    return __primitive("array_set", this, i, val);
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

var EOF: integer;
var errno: integer;

function strerror(errno: integer) : string {
  return __primitive("strerror", errno);
}

var chpl_input_filename: string;
var chpl_input_lineno: integer;

pragma "rename fprintf"
function fprintf(fp: CFILEPTR, fmt: string, val) : integer {
  return __primitive("fprintf", fp.FILEptr, fmt, val);
}

pragma "rename fscanf"
function fscanf(fp: CFILEPTR, fmt: string, inout val) : integer {
  return __primitive("fscanf", fp.FILEptr, string, val);
}

function _chpl_fwrite_float_help(fp: CFILEPTR, val: float) : void {
  __primitive("fprintf", fp.FILEptr, "%s", _tostring(val, ""));
}

function _chpl_fread_string_help(fp: CFILEPTR, inout val: string) : void {
  __primitive("fscanf", "%g", val);
}

function _readLitChar(fp: CFILEPTR, val: string, ignoreWhiteSpace: integer) : integer {
  return __primitive("fscanf", "%s", val);
}

function _classReadError() {}

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

record _methodTokenType { }
var _methodToken : _methodTokenType;

record _setterTokenType { }
var _setterToken : _setterTokenType;

pragma "rename _copy_string" function =(a : string, b : any) : string { 
  return __primitive("copy_string", b);
}

pragma "split unique"
pragma "builtin" function =(a : numeric, b : any) { 
  return __primitive("cast", a, b);
}

pragma "split unique"
pragma "builtin" function =(a : any, b : any) { 
  return b; 
}

pragma "split unique"
pragma "rename _chpl_alloc"
function _chpl_alloc(t, description:string) { return __primitive("chpl_alloc", t); }

function _init_string() { return ""; }

function _INIT_CONFIG(inout v, v_type, chapel_name, module_name);
