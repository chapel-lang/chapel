//
// Functions on _file primitive type, the C file pointer type
//

pragma "no codegen" var chpl_input_filename: string;
pragma "no codegen" var chpl_input_lineno: int;

pragma "inline" fun _get_errno() return __primitive("get_errno");
pragma "inline" fun _get_eof() return __primitive("get_eof");
pragma "inline" fun _get_stdin() return __primitive("get_stdin");
pragma "inline" fun _get_stdout() return __primitive("get_stdout");
pragma "inline" fun _get_stderr() return __primitive("get_stderr");
pragma "inline" fun _get_nullfile() return __primitive("get_nullfile");

pragma "inline" fun _copy(x: _file) return x;
pragma "inline" fun =(a: _file, b: _file) return b;
pragma "inline" fun ==(a: _file, b: _file) return __primitive("==", a, b);
pragma "inline" fun !=(a: _file, b: _file) return __primitive("!=", a, b);

pragma "inline" fun _fopen(filename: string, mode: string)
  return __primitive("fopen", filename, mode);

pragma "inline" fun _fclose(fp: _file)
  return __primitive("fclose", fp);

pragma "inline" fun fprintf(fp: _file, fmt: string, val)
  return __primitive("fprintf", fp, fmt, val);

pragma "inline" fun fscanf(fp: _file, fmt: string, inout val)
  return __primitive("fscanf", fp, fmt, val);

pragma "inline" fun _string_fscanf(fp: _file)
  return __primitive("string_fscanf", fp);

pragma "inline" fun _readLitChar(fp: _file, val: string, ignoreWhiteSpace: bool)
  return __primitive("readLit", fp, val, ignoreWhiteSpace);




pragma "rename _chpl_fflush"
fun fflush(f: file) {
  fflush(f.fp);
}

pragma "rename _chpl_fflush_nil"
fun fflush(f: _nilType) {
  halt("***Error: called fflush on nil***");
}

pragma "rename _chpl_assert_no_args"
fun assert(test: bool) {
  if (!test) {
    _writeAssertFailed();
    if (!chpl_input_lineno) {
      fwriteln(stderr);
    }
    exit(0);
  }
}

pragma "rename _chpl_assert"
fun assert(test: bool, args ...?numArgs) {
  if (!test) {
    _writeAssertFailed();
    if (!chpl_input_lineno) {
      fwrite(stderr, ": ");
    }
    for param i in 1..numArgs {
      fwrite(stderr, args(i));
    }
    fwriteln(stderr);
    exit(0);
  }
}

fun _writeAssertFailed() {
  fflush(stdout);
  fwrite(stderr, "Assertion failed");
  if (chpl_input_lineno) {
    fwriteln(stderr, ": ", chpl_input_filename, ":", chpl_input_lineno);
  }     
}

pragma "rename _chpl_halt_no_args"
fun halt() {
  _writeHaltReached();
  if(!chpl_input_lineno) {
    fwriteln(stderr);
  }
  exit(0);
}

pragma "rename _chpl_halt"
fun halt(args ...?numArgs) {
  _writeHaltReached();
  if (!chpl_input_lineno) {
    fwrite(stderr, ": ");
  }
  for param i in 1..numArgs {
    fwrite(stderr, args(i));
  }
  fwriteln(stderr);
  exit(0);
}

fun _writeHaltReached() {
  fflush(stdout);
  fwrite(stderr, "Halt reached");
  if (chpl_input_lineno) {
    fwriteln(stderr, ": ", chpl_input_filename, ":", chpl_input_lineno);
  }
}


class file {
  var filename : string = "";
  var mode : string = "r";
  var path : string = ".";
  var fp : _file;

  fun open {
    if (this == stdin || this == stdout || this == stderr) {
      halt("***Error: It is not necessary to open \"", filename, "\"***");
    }

    var fullFilename = path + "/" + filename;
    fp = _fopen(fullFilename, mode);            

    if (fp == _get_nullfile()) {
      halt("***Error: Unable to open \"", fullFilename, "\": ", 
           _get_errno(), "***");
    }
  }

  fun isOpen: bool {
    var openStatus: bool = false;
    if (fp != _get_nullfile()) {
      openStatus = true;
    }
    return openStatus;
  }

  fun close {
    if (this == stdin || this == stdout || this == stderr) {
      halt("***Error: You may not close \"", filename, "\"***");
    }
    if (fp == _get_nullfile()) {
      var fullFilename = path + "/" + filename;
      halt("***Error: Trying to close \"", fullFilename, 
           "\" which isn't open***");
    }
    var returnVal: int = _fclose(fp);
    if (returnVal < 0) {
      var fullFilename = path + "/" + filename;
      halt("***Error: The close of \"", fullFilename, "\" failed: ", 
           _get_errno(), "***");
    }
    fp = _get_nullfile();
  }
}

const stdin  : file = file("stdin", "r", "/dev", _get_stdin());
const stdout : file = file("stdout", "w", "/dev", _get_stdout());
const stderr : file = file("stderr", "w", "/dev", _get_stderr());


fun fopenError(f: file, isRead: bool) {
  var fullFilename:string = f.path + "/" + f.filename;
  if (isRead) {
    halt("***Error: You must open \"", fullFilename, 
         "\" before trying to read from it***");
  } else {
    halt("***Error: You must open \"", fullFilename, 
         "\" before trying to write to it***");
  }
}


fun fprintfError() {
  halt("***Error: Write failed: ", _get_errno(), "***");
}


fun fscanfError() {
  halt("***Error: Read failed: ", _get_errno(), "***");
}


pragma "rename _chpl_fread_int" 
fun fread(f: file = stdin, inout val: int) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%lld", val);
    if (returnVal == _get_eof()) {
      halt("***Error: Read failed: EOF***");
    }
    if (returnVal < 0) {
      fscanfError();
    } else if (returnVal == 0) {
      halt("***Error: No int was read***");
    }
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_int"
fun fwrite(f: file, val: int) {
  if (f.isOpen) {
    var returnVal: int = fprintf(f.fp, "%lld", val);
    if (returnVal < 0) {
      fprintfError();
    } else if (returnVal == 0) {
      halt("***Error: No int was written***");
    }
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_uint" 
fun fread(f: file = stdin, inout val: uint) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%llu", val);
    if (returnVal == _get_eof()) {
      halt("***Error: Read failed: EOF***");
    }
    if (returnVal < 0) {
      fscanfError();
    } else if (returnVal == 0) {
      halt("***Error: No int was read***");
    }
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_uint"
fun fwrite(f: file, val: uint) {
  if (f.isOpen) {
    var returnVal: int = fprintf(f.fp, "%llu", val);
    if (returnVal < 0) {
      fprintfError();
    } else if (returnVal == 0) {
      halt("***Error: No int was written***");
    }
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_float"
fun fread(f: file = stdin, inout val: float) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%lg", val);
    if (returnVal == _get_eof()) {
      halt("***Error: Read failed: EOF***");
    }
    if (returnVal < 0) {
      fscanfError();
    } else if (returnVal == 0) {
      halt("***Error: No float was read***");
    }
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_float"
fun fwrite(f: file, val: float) {
  if (f.isOpen) {
    var returnVal : int;
    var str : string = val:"%g";
    if (_string_contains(str, ".") || _string_contains(str, "e")) {
      returnVal = fprintf(f.fp, "%s", str);
    } else {
      returnVal = fprintf(f.fp, "%s.0", str);
    }
    if returnVal < 0 then
      fprintfError();
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_complex"
fun fread(f: file = stdin, inout val: complex) {
  var realPart: float;
  var imagPart: float;
  var imagI: string;
  var matchingCharWasRead: int;
  var isNeg: bool;

  fread(f, realPart);
  matchingCharWasRead = _readLitChar(f.fp, "+", true);
  if (matchingCharWasRead != 1) {
    matchingCharWasRead = _readLitChar(f.fp, "-", true);
    if (matchingCharWasRead != 1) {
      halt("***Error: Incorrect format for complex numbers***");
    }
    isNeg = true;
  }

  fread(f, imagPart);
  matchingCharWasRead = _readLitChar(f.fp, "i", false);
  if (matchingCharWasRead != 1) {
    halt("***Error: Incorrect format for complex numbers***");
  }

  val.real = realPart;
  if (isNeg) {
    val.imag = -imagPart;
  } else {
    val.imag = imagPart;
  }
}


pragma "rename _chpl_fwrite_complex"
fun fwrite(f: file, val: complex) {
  if (f.isOpen) {
     fwrite(f, val.real);
     if (val.imag >= 0) {
       fwrite(f, " + ", val.imag, "i");
     } else {
       fwrite(f, " - ", -val.imag, "i");
     }
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_string"
fun fread(f: file = stdin, inout val: string) {
  if (f.isOpen) {
    val = _string_fscanf(f.fp);
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_string"
fun fwrite(f: file, val: string) {
  if (f.isOpen) {
    var returnVal: int = fprintf(f.fp, "%s", val);
    if (returnVal < 0) {
      fprintfError();
    } 
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_bool" 
fun fread(f: file = stdin, inout val: bool) {
  if (f.isOpen) {
    var valString : string = _string_fscanf(f.fp);
    if (valString == "true") {
      val = true;
    } else if (valString == "false") {
      val = false;
    } else {
      halt("***Error: Not of bool type***");
    }
  } else {
    fopenError(f, isRead = true);
  }
}

pragma "rename _chpl_fwrite_bool"
fun fwrite(f: file, val: bool) {
  if (f.isOpen) {
    var returnVal: int;
    if (val == true) {
      returnVal = fprintf(f.fp, "%s", "true");
    } else {
      returnVal = fprintf(f.fp, "%s", "false");
    }
    if (returnVal < 0) { 
      fprintfError();
    }
  } else {
    fopenError(f, isRead = false);
  }
}

pragma "rename _chpl_fwrite_syncvar"
fun fwrite(f: file, sv: _syncvar) {
  fwrite(f, sv.value);
}

pragma "rename _chpl_fwrite_nil" 
fun fwrite(f: file, x : _nilType) : void {
  if (f.isOpen) {
    var returnVal: int = fprintf(f.fp, "%s", "nil");
    if (returnVal < 0) {
      fprintfError();
    }
  } else {
    fopenError(f, isRead = false);
  }
}

fun fwrite(f: file, args ...?numArgs) {
  for param i in 1..numArgs {
    fwrite(f, args(i));
  }
}

fun fwriteln(f: file, args ...?numArgs) {
  fwrite(f, (...args));
  fwriteln(f);
}

fun fwriteln(f: file) {
  fwrite(f, "\n");
}

fun write(args ...?numArgs) {
  fwrite(stdout, (...args));
}

fun writeln(args ...?numArgs) {
  fwriteln(stdout, (...args));
}

fun writeln() {
  fwriteln(stdout);
}
