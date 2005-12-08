class file {
  var filename : string = "";
  var mode : string = "r";
  var path : string = ".";
  var fp : CFILEPTR = _NULLCFILEPTR;

  function open {
    if (fp == _STDINCFILEPTR or fp == _STDOUTCFILEPTR or  
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

  function isOpen: boolean {
    var openStatus: boolean = false;
    if (fp != _NULLCFILEPTR) {
      openStatus = true;
    }
    return openStatus;
  }

  function close {
    if (fp == _STDINCFILEPTR or fp == _STDOUTCFILEPTR or  
        fp == _STDERRCFILEPTR) {
      halt("***Error: You may not close \"", filename, "\"***");
    }
    if (fp == _NULLCFILEPTR) {
      var fullFilename = path + "/" + filename;
      halt("***Error: Trying to close \"", fullFilename, 
           "\" which isn't open***");
    }
    var returnVal: integer = _fclose(fp);
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


function fopenError(f: file, isRead: boolean) {
  var fullFilename:string = f.path + "/" + f.filename;
  if (isRead) {
    halt("***Error: You must open \"", fullFilename, 
         "\" before trying to read from it***");
  } else {
    halt("***Error: You must open \"", fullFilename, 
         "\" before trying to write to it***");
  }
}


function fprintfError() {
  halt("***Error: Write failed: ", strerror(errno), "***");
}


function fscanfError() {
  halt("***Error: Read failed: ", strerror(errno), "***");
}


pragma "rename _chpl_fwriteln"
function fwriteln(f: file = stdout) {
  if (f.isOpen) {
    var returnVal: integer = fprintf(f.fp, "%s", "\n");
    if (returnVal < 0) {
      fprintfError();
    } 
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_integer" 
function fread(f: file = stdin, inout val: integer) {
  if (f.isOpen) {
    var returnVal: integer = fscanf(f.fp, "%lld", val);
    if (returnVal < 0) {
      fscanfError();
    } else if (returnVal == 0) {
      halt("***Error: No integer was read***");
    }
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_integer"
function fwrite(f: file = stdout, val: integer) {
  if (f.isOpen) {
    var returnVal: integer = fprintf(f.fp, "%lld", val);
    if (returnVal < 0) {
      fprintfError();
    } else if (returnVal == 0) {
      halt("***Error: No integer was written***");
    }
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_float"
function fread(f: file = stdin, inout val: float) {
  if (f.isOpen) {
    var returnVal: integer = fscanf(f.fp, "%lg", val);
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
function fwrite(f: file = stdout, val: float) {
  if (f.isOpen) {
     _chpl_fwrite_float_help(f.fp, val);
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_string"
function fread(f: file = stdin, inout val: string) {
  if (f.isOpen) {
    _chpl_fread_string_help(f.fp, val);
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_string"
function fwrite(f: file = stdout, val: string) {
  if (f.isOpen) {
    var returnVal: integer = fprintf(f.fp, "%s", val);
    if (returnVal < 0) {
      fprintfError();
    } 
  } else {
    fopenError(f, isRead = false);
  }
}


pragma "rename _chpl_fread_boolean" 
function fread(f: file = stdin, inout val: boolean) {
  if (f.isOpen) {
    var valString : string;
    _chpl_fread_string_help(f.fp, valString);
    if (valString == "true") {
      val = true;
    } else if (valString == "false") {
      val = false;
    } else {
      halt("***Error: Not of boolean type***");
    }
  } else {
    fopenError(f, isRead = true);
  }
}


pragma "rename _chpl_fwrite_boolean"
function fwrite(f: file = stdout, val: boolean) {
  if (f.isOpen) {
    var returnVal: integer;
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
function fwrite(f: file = stdout, x : _nilType) : void {
  if (f.isOpen) {
    var returnVal: integer = fprintf(f.fp, "%s", "nil");
    if (returnVal < 0) {
      fprintfError();
    }
  } else {
    fopenError(f, isRead = false);
  }
}


function write() {
  halt("***Error: This should never be called.  All write calls should be converted to fwrites***");
}


function writeln() { 
  halt("***Error: This should never be called.  All writeln calls should be converted to fwritelns***");
}

function read() {
  halt("***Error: This should never be called.  All read calls should be converted to freads***");
}


