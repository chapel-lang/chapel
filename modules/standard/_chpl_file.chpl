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
  var fp : CFILEPTR = _NULLCFILEPTR;

  fun open {
    if (fp == _STDINCFILEPTR || fp == _STDOUTCFILEPTR ||  
        fp == _STDERRCFILEPTR) {
      halt("***Error: It is not necessary to open \"", filename, "\"***");
    }
    if (fp != _NULLCFILEPTR) {
      this.close;
    }

    var fullFilename = path + "/" + filename;
    fp = _fopen(fullFilename, mode);            

    if (fp == _NULLCFILEPTR) {
      halt("***Error: Unable to open \"", fullFilename, "\": ", 
           strerror(errno), "***");
    }
  }

  fun isOpen: bool {
    var openStatus: bool = false;
    if (fp != _NULLCFILEPTR) {
      openStatus = true;
    }
    return openStatus;
  }

  fun close {
    if (fp == _STDINCFILEPTR || fp == _STDOUTCFILEPTR ||  
        fp == _STDERRCFILEPTR) {
      halt("***Error: You may not close \"", filename, "\"***");
    }
    if (fp == _NULLCFILEPTR) {
      var fullFilename = path + "/" + filename;
      halt("***Error: Trying to close \"", fullFilename, 
           "\" which isn't open***");
    }
    var returnVal: int = _fclose(fp);
    if (returnVal < 0) {
      var fullFilename = path + "/" + filename;
      halt("***Error: The close of \"", fullFilename, "\" failed: ", 
           strerror(errno), "***");
    }
    fp = _NULLCFILEPTR;
  }
}

const stdin  : file = file("stdin", "r", "/dev", _STDINCFILEPTR);
const stdout : file = file("stdout", "w", "/dev", _STDOUTCFILEPTR);
const stderr : file = file("stderr", "w", "/dev", _STDERRCFILEPTR);


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
  halt("***Error: Write failed: ", strerror(errno), "***");
}


fun fscanfError() {
  halt("***Error: Read failed: ", strerror(errno), "***");
}


pragma "rename _chpl_fwriteln"
fun fwriteln(f: file = stdout) {
  if (f.isOpen) {
    var returnVal: int = fprintf(f.fp, "%s", "\n");
    if (returnVal < 0) {
      fprintfError();
    } 
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_int" 
fun fread(f: file = stdin, inout val: int) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%lld", val);
    if (returnVal == EOF) {
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
fun fwrite(f: file = stdout, val: int) {
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


pragma "rename _chpl_fread_float"
fun fread(f: file = stdin, inout val: float) {
  if (f.isOpen) {
    var returnVal: int = fscanf(f.fp, "%lg", val);
    if (returnVal == EOF) {
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
fun fwrite(f: file = stdout, val: float) {
  if (f.isOpen) {
     _chpl_fwrite_float_help(f.fp, val);
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_string"
fun fread(f: file = stdin, inout val: string) {
  if (f.isOpen) {
    _chpl_fread_string_help(f.fp, val);
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_string"
fun fwrite(f: file = stdout, val: string) {
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
    var valString : string;
    _chpl_fread_string_help(f.fp, valString);
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
fun fwrite(f: file = stdout, val: bool) {
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


pragma "rename _chpl_fwrite_nil" 
fun fwrite(f: file = stdout, x : _nilType) : void {
  if (f.isOpen) {
    var returnVal: int = fprintf(f.fp, "%s", "nil");
    if (returnVal < 0) {
      fprintfError();
    }
  } else {
    fopenError(f, isRead = false);
  }
}


fun write() {
  halt("***Error: This should never be called.  All write calls should be converted to fwrites***");
}


fun writeln() { 
  halt("***Error: This should never be called.  All writeln calls should be converted to fwritelns***");
}

fun read() {
  halt("***Error: This should never be called.  All read calls should be converted to freads***");
}


