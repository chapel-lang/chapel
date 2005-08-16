class file {
  var filename : string = "";
  var mode : string = "r";
  var path : string = ".";
  var fp : CFILEPTR = _NULLCFILEPTR;

  function open {
    if (fp != _NULLCFILEPTR) {
      this.close;
    }
    var fullFilename = path + "/" + filename;
    fp = _fopen(fullFilename, mode);            
    if (fp == _NULLCFILEPTR) {
      halt("Unable to open \"", fullFilename, "\"");
    }
  }

  function close {
    _fclose(fp);
  }

}
