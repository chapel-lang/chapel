use IO, Regex;

// eof errors:
readStringFromEOF(true, openReader("maxSizeInput.txt"));
readStringFromEOF(false, openReader("maxSizeInput.txt"));
readBytesFromEOF(true, openReader("maxSizeInput.txt"));
readBytesFromEOF(false, openReader("maxSizeInput.txt"));

// (the BadFormatError case is tested by 'readThroughMaxSize.chpl')

proc readStringFromEOF(param byRef: bool, fr) where byRef == true {
  fr.readAll();
  var s: string;
  writeln(fr.readThrough("x", s));
}

proc readStringFromEOF(param byRef: bool, fr) where byRef == false {
  fr.readAll();
  try {
    fr.readThrough("x");
  } catch e {
    writeln(e);
  }
}

proc readBytesFromEOF(param byRef: bool, fr) where byRef == true {
  fr.readAll();
  var b: bytes;
  writeln(fr.readThrough(b"x", b));
}

proc readBytesFromEOF(param byRef: bool, fr) where byRef == false {
  fr.readAll();
  try {
    fr.readThrough(b"x");
  } catch e {
    writeln(e);
  }
}
