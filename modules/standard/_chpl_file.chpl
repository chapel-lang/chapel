class file {
  var filename : string = "";
  var mode : string = "r";
  var path : string = ".";
  var fp : CFILEPTR;

  function open {
    fp = _fopen(filename, mode);
    if (fp == _NULLCFILEPTR) {
      halt("Unable to open \"", filename, "\"");
    }
  }

  function close {
    _fclose(fp);
  }

}
