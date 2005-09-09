class file {
  var filename : string = "";
  var mode : string = "r";
  var path : string = ".";
  var fp : CFILEPTR = _NULLCFILEPTR;

  function open {
    if (fp == _STDINCFILEPTR or fp == _STDOUTCFILEPTR or  
        fp == _STDERRCFILEPTR) {
      halt("It is not necessary to open \"", filename, "\".");
    }
    if (fp != _NULLCFILEPTR) {
      this.close;
    }

    var fullFilename = path + "/" + filename;
    fp = _fopen(fullFilename, mode);            

    if (fp == _NULLCFILEPTR) {
      halt("Unable to open \"", fullFilename, "\".");
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
      halt("You may not close \"", filename, "\".");
    }
    if (fp == _NULLCFILEPTR) {
      var fullFilename = path + "/" + filename;
      halt("Trying to close \"", fullFilename, "\" which isn't open.");
    }

    _fclose(fp);
    fp = _NULLCFILEPTR;
  }
}

const stdin  : file = file(filename = "stdin",  mode = "r", path = "/dev", 
                           fp = _STDINCFILEPTR);
const stdout : file = file(filename = "stdout", mode = "w", path = "/dev", 
                           fp = _STDOUTCFILEPTR);
const stderr : file = file(filename = "stderr", mode = "w", path = "/dev", 
                           fp = _STDERRCFILEPTR);


pragma "rename _chpl_fwrite_integer"
function fwrite(f: file = stdout, val: integer) {
  if (f.isOpen) {
    fprintf(f.fp, "%lld", val);
  } else {
    var fullFilename = f.path + "/" + f.filename;
    halt("You must open \"", fullFilename, "\" before writing to it.");    
  }
}
