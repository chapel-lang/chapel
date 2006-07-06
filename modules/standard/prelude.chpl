pragma "no codegen";

class object { }
record value { }

var _object : object;
var _value : value();

fun typeof(x) { }

-- I/O

class CFILEPTR { 
  var FILEptr : int;
}

const _NULLCFILEPTR   : CFILEPTR = CFILEPTR(-1);
const _STDINCFILEPTR  : CFILEPTR = CFILEPTR(0);
const _STDOUTCFILEPTR : CFILEPTR = CFILEPTR(1);
const _STDERRCFILEPTR : CFILEPTR = CFILEPTR(2);

pragma "rename fopen" fun _fopen(filename, mode : string) : CFILEPTR {
  return CFILEPTR(__primitive("fopen", filename, mode));
}

pragma "rename fclose" fun _fclose(fp : CFILEPTR) : int {
  return __primitive("fclose", fp.FILEptr);
}

var EOF: int;
var errno: int;

fun strerror(errno: int) : string {
  return __primitive("strerror", errno);
}

var chpl_input_filename: string;
var chpl_input_lineno: int;

pragma "rename fprintf"
fun fprintf(fp: CFILEPTR, fmt: string, val) : int {
  return __primitive("fprintf", fp.FILEptr, fmt, val);
}

pragma "rename fscanf"
fun fscanf(fp: CFILEPTR, fmt: string, inout val) : int {
  return __primitive("fscanf", fp.FILEptr, string, val);
}

fun _chpl_fwrite_float_help(fp: CFILEPTR, val: float) : void {
  __primitive("fprintf", fp.FILEptr, "%s", _tostring(val, ""));
}

fun _chpl_fread_string_help(fp: CFILEPTR, inout val: string) : void {
  __primitive("fscanf", "%g", val);
}

fun _readLitChar(fp: CFILEPTR, val: string, ignoreWhiteSpace: int) : int {
  return __primitive("fscanf", "%s", val);
}

fun _classReadError() {}

pragma "rename _chpl_tostring_bool"
fun _tostring(x : bool, format : string) : string {
  return __primitive("to_string", format, x);
}

pragma "rename _chpl_tostring_int"
fun _tostring(x : int, format : string) : string {
  return __primitive("to_string", format, x);
}

pragma "rename _chpl_tostring_float"
fun _tostring(x : float, format : string) : string {
         return __primitive("to_string", format, x);
}

pragma "rename _chpl_tostring_complex"
fun _tostring(x : complex, format : string) : string {
         return __primitive("to_string", format, x);
}

pragma "rename _chpl_alloc"
fun _chpl_alloc(type t, description:string) { return __primitive("chpl_alloc", t); }

fun _init_string() { return ""; }
