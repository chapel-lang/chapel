pragma "no codegen";

class object { }
class value { }

function typeof(x) { }

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
