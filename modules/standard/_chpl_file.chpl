//
// Functions on _file primitive type, the C file pointer type
//

pragma "no codegen" var chpl_input_filename: string;
pragma "no codegen" var chpl_input_lineno: int;

pragma "inline" def _get_errno() return __primitive("get_errno");
pragma "inline" def _get_eof() return __primitive("get_eof");
pragma "inline" def _get_stdin() return __primitive("get_stdin");
pragma "inline" def _get_stdout() return __primitive("get_stdout");
pragma "inline" def _get_stderr() return __primitive("get_stderr");
pragma "inline" def _get_nullfile() return __primitive("get_nullfile");

pragma "inline" def _copy(x: _file) return x;
pragma "inline" def =(a: _file, b: _file) return b;
pragma "inline" def ==(a: _file, b: _file) return __primitive("==", a, b);
pragma "inline" def !=(a: _file, b: _file) return __primitive("!=", a, b);

pragma "inline" def _fopen(filename: string, mode: string)
  return __primitive("fopen", filename, mode);

pragma "inline" def _fclose(fp: _file)
  return __primitive("fclose", fp);

pragma "inline" def fprintf(fp: _file, fmt: string, val)
  return __primitive("fprintf", fp, fmt, val);

pragma "inline" def fscanf(fp: _file, fmt: string, inout val)
  return __primitive("fscanf", fp, fmt, val);

pragma "inline" def _string_fscanf(fp: _file)
  return __primitive("string_fscanf", fp);

pragma "inline" def _readLitChar(fp: _file, val: string, ignoreWhiteSpace: bool)
  return __primitive("readLit", fp, val, ignoreWhiteSpace);




pragma "rename _chpl_fflush"
def fflush(f: file) {
  fflush(f.fp);
}

pragma "rename _chpl_fflush_nil"
def fflush(f: _nilType) {
  halt("***Error: called fflush on nil***");
}

pragma "rename _chpl_assert_no_args"
def assert(test: bool) {
  if (!test) {
    _writeAssertFailed();
    if (!chpl_input_lineno) {
      fwriteln(stderr);
    }
    exit(0);
  }
}

pragma "rename _chpl_assert"
def assert(test: bool, args ...?numArgs) {
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

def _writeAssertFailed() {
  fflush(stdout);
  fwrite(stderr, "Assertion failed");
  if (chpl_input_lineno) {
    fwriteln(stderr, ": ", chpl_input_filename, ":", chpl_input_lineno);
  }     
}

pragma "rename _chpl_halt_no_args"
def halt() {
  _writeHaltReached();
  if(!chpl_input_lineno) {
    fwriteln(stderr);
  }
  exit(0);
}

pragma "rename _chpl_halt"
def halt(args ...?numArgs) {
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

def _writeHaltReached() {
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
  var writeLock : sync uint;    // for serializing fwrite output

  def open {
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

  def _checkFileStateChangeLegality(state) {
    if (isOpen) {
      halt("Cannot change ", state, " of file ", path, "/", filename, 
           " while it is open");
    }
  }

/*
  // BLC: These next three setters can be removed if we can
  // special-case setters to not call themselves recursively
  // when the setter has the same name as a field

  def filename {
    return _filename;
  }

  def =filename(newfilename) {
    _checkFileStateChangeLegality("filename");
  }

  def path {
    return _path;
  }

  def =path(newpath) {
    _checkFileStateChangeLegality("path");
    path = newpath;
  }

  def mode {
    return _mode;
  }

  def =mode(newmode: string) {
    _checkFileStateChangeLegality("mode");
    mode = newmode;
  }
*/

  def isOpen: bool {
    var openStatus: bool = false;
    if (fp != _get_nullfile()) {
      openStatus = true;
    }
    return openStatus;
  }

  def close {
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


def _fopenError(f: file, isRead: bool) {
  var fullFilename:string = f.path + "/" + f.filename;
  if (isRead) {
    halt("***Error: You must open \"", fullFilename, 
         "\" before trying to read from it***");
  } else {
    halt("***Error: You must open \"", fullFilename, 
         "\" before trying to write to it***");
  }
}


def _fmodeError(f: file, isRead: bool) {
  var fullFilename: string = f.path + "/" + f.filename;
  var modestring = if (isRead) then "reading" else "writing";
  halt("***Error: ", fullFilename, " not open for ", modestring, "***");
}


def _fprintfError() {
  halt("***Error: Write failed: ", _get_errno(), "***");
}


def _fscanfError() {
  halt("***Error: Read failed: ", _get_errno(), "***");
}


pragma "rename _chpl_fread_int" 
def fread(f: file = stdin, inout val: int) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%lld", val);
    if (returnVal == _get_eof()) {
      halt("***Error: Read failed: EOF***");
    }
    if (returnVal < 0) {
      _fscanfError();
    } else if (returnVal == 0) {
      halt("***Error: No int was read***");
    }
  } else {
    _fopenError(f, isRead = true);
  }
}


def _fwrite_lock( f: file) {
  var me: uint = __primitive( "thread_id");
  if (isFull( f.writeLock)) {
    if (readXX( f.writeLock) == me) {
      return false;
    }
  }
  f.writeLock = me;
  return true;
}

def _fwrite_unlock( f: file) {
  writeFE( f.writeLock, 0);  // writeXE works also as it should be full before
}


pragma "rename _chpl_fread_uint" 
def fread(f: file = stdin, inout val: uint) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%llu", val);
    if (returnVal == _get_eof()) {
      halt("***Error: Read failed: EOF***");
    }
    if (returnVal < 0) {
      _fscanfError();
    } else if (returnVal == 0) {
      halt("***Error: No int was read***");
    }
  } else {
    _fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fread_float"
def fread(f: file = stdin, inout val: float) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%lg", val);
    if (returnVal == _get_eof()) {
      halt("***Error: Read failed: EOF***");
    }
    if (returnVal < 0) {
      _fscanfError();
    } else if (returnVal == 0) {
      halt("***Error: No float was read***");
    }
  } else {
    _fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fread_complex"
def fread(f: file = stdin, inout val: complex) {
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


pragma "rename _chpl_fread_string"
def fread(f: file = stdin, inout val: string) {
  if (f.isOpen) {
    val = _string_fscanf(f.fp);
  } else {
    _fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fread_bool" 
def fread(f: file = stdin, inout val: bool) {
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
    _fopenError(f, isRead = true);
  }
}

pragma "rename _chpl_fwrite_string"
def fwrite(f: file, val: string) {
  var need_release: bool = _fwrite_lock( f);

  if (!f.isOpen) {
    _fopenError(f, isRead = false);
  }
  if (f.mode != "w") {
    _fmodeError(f, isRead = false);
  }
  var returnVal = fprintf(f.fp, "%s", val);
  if (returnVal < 0) {
    _fprintfError();
  } 

  if (need_release) { _fwrite_unlock( f); }
}


pragma "rename _chpl_fwrite_nil" 
def fwrite(f: file, x : _nilType) : void {
  var need_release: bool = _fwrite_lock( f);

  if (f.isOpen) {
    var returnVal: int = fprintf(f.fp, "%s", "nil");
    if (returnVal < 0) {
      _fprintfError();
    } else if (returnVal == 0) {
      halt("*** Error: No value was written***");
    }
  } else {
    _fopenError(f, isRead = false);
  }
  if (need_release) { _fwrite_unlock( f); }
}


def fwrite(f: file, args ...?numArgs) {
  var need_release: bool = _fwrite_lock( f);
  for param i in 1..numArgs {
    fwrite(f, args(i));
  }
  if (need_release) { _fwrite_unlock( f); }
}

def fwriteln(f: file, args ...?numArgs) {
  var need_release: bool = _fwrite_lock( f);
  fwrite(f, (...args));
  fwriteln(f);
  if (need_release) { _fwrite_unlock( f); }
}

def fwriteln(f: file) {
  var need_release: bool = _fwrite_lock( f);
  fwrite(f, "\n");
  if (need_release) { _fwrite_unlock( f); }
}


def write(args ...?numArgs) {
  fwrite(stdout, (...args));
}

def writeln(args ...?numArgs) {
  fwriteln(stdout, (...args));
}

def writeln() {
  fwriteln(stdout);
}
