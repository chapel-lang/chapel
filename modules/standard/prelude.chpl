pragma "no codegen";

class object { }
class value { }

function typeof(x) { }

-- I/O

class CFILEPTR { 
  var FILEptr : int;
}

const _NULLCFILEPTR   : CFILEPTR = CFILEPTR(-1);
const _STDINCFILEPTR  : CFILEPTR = CFILEPTR(0);
const _STDOUTCFILEPTR : CFILEPTR = CFILEPTR(1);
const _STDERRCFILEPTR : CFILEPTR = CFILEPTR(2);

pragma "rename fopen" function _fopen(filename, mode : string) : CFILEPTR {
  return CFILEPTR(__primitive("fopen", filename, mode));
}

pragma "rename fclose" function _fclose(fp : CFILEPTR) : int {
  return __primitive("fclose", fp.FILEptr);
}

var EOF: int;
var errno: int;

function strerror(errno: int) : string {
  return __primitive("strerror", errno);
}

var chpl_input_filename: string;
var chpl_input_lineno: int;

pragma "rename fprintf"
function fprintf(fp: CFILEPTR, fmt: string, val) : int {
  return __primitive("fprintf", fp.FILEptr, fmt, val);
}

pragma "rename fscanf"
function fscanf(fp: CFILEPTR, fmt: string, inout val) : int {
  return __primitive("fscanf", fp.FILEptr, string, val);
}

function _chpl_fwrite_float_help(fp: CFILEPTR, val: float) : void {
  __primitive("fprintf", fp.FILEptr, "%s", _tostring(val, ""));
}

function _chpl_fread_string_help(fp: CFILEPTR, inout val: string) : void {
  __primitive("fscanf", "%g", val);
}

function _readLitChar(fp: CFILEPTR, val: string, ignoreWhiteSpace: int) : int {
  return __primitive("fscanf", "%s", val);
}

function _classReadError() {}

pragma "rename _chpl_tostring_bool"
function _tostring(x : bool, format : string) : string {
  return __primitive("to_string", format, x);
}

pragma "rename _chpl_tostring_int"
function _tostring(x : int, format : string) : string {
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
