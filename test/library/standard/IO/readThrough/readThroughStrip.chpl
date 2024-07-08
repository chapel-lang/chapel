use IO, List;

// string
writeln(readIntoList(true, openReader("./listInput.txt", locking=false), ","));
writeln(readIntoList(false, openReader("./listInput.txt", locking=false), ","));

writeln(readIntoList(true, openReader("./listInputTrailing.txt", locking=false), ","));
writeln(readIntoList(false, openReader("./listInputTrailing.txt", locking=false), ","));

writeln(readIntoList(true, openReader("./listInputWeird.txt", locking=false), ",,||,,"));
writeln(readIntoList(false, openReader("./listInputWeird.txt", locking=false), ",,||,,"));

// bytes
writeln(readIntoList(true, openReader("./listInput.txt", locking=false), b","));
writeln(readIntoList(false, openReader("./listInput.txt", locking=false), b","));

writeln(readIntoList(true, openReader("./listInputTrailing.txt", locking=false), b","));
writeln(readIntoList(false, openReader("./listInputTrailing.txt", locking=false), b","));

writeln(readIntoList(true, openReader("./listInputWeird.txt", locking=false), b",,||,,"));
writeln(readIntoList(false, openReader("./listInputWeird.txt", locking=false), b",,||,,"));

proc readIntoList(param reuseBuffer, r, separator: ?t) where reuseBuffer == true {
  var s : t,
      l = new list(int);
  while r.readThrough(separator, s, stripSeparator=true) {
    // take care of potential newlines (see '/regex/readCommaOrNewline.chpl')
    if !s.strip().isEmpty()
      then l.pushBack(s:int);
  }
  return l;
}

proc readIntoList(param reuseBuffer, r, separator) where reuseBuffer == false {
  var l = new list(int);

  while true {
    try {
      // take care of potential newlines (see '/regex/readCommaOrNewline.chpl')
      const s = r.readThrough(separator, stripSeparator=true);
      if !s.strip().isEmpty() then l.pushBack(s:int);
    } catch {
      break;
    }
  }

  return l;
}
