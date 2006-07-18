pragma "no codegen";

-- I/O

const _NULLCFILEPTR   : _file;
const _STDINCFILEPTR  : _file;
const _STDOUTCFILEPTR : _file;
const _STDERRCFILEPTR : _file;

var EOF: int;

var chpl_input_filename: string;
var chpl_input_lineno: int;

fun _classReadError() {}

fun _init_string() { return ""; }
