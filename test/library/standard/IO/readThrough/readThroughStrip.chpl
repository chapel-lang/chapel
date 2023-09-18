use IO, List;

// string
writeln(readIntoList(true, openReader("./listInput.txt"), ","));
writeln(readIntoList(false, openReader("./listInput.txt"), ","));

writeln(readIntoList(true, openReader("./listInputTrailing.txt"), ","));
writeln(readIntoList(false, openReader("./listInputTrailing.txt"), ","));

writeln(readIntoList(true, openReader("./listInputWeird.txt"), ",,||,,"));
writeln(readIntoList(false, openReader("./listInputWeird.txt"), ",,||,,"));

// bytes
writeln(readIntoList(true, openReader("./listInput.txt"), b","));
writeln(readIntoList(false, openReader("./listInput.txt"), b","));

writeln(readIntoList(true, openReader("./listInputTrailing.txt"), b","));
writeln(readIntoList(false, openReader("./listInputTrailing.txt"), b","));

writeln(readIntoList(true, openReader("./listInputWeird.txt"), b",,||,,"));
writeln(readIntoList(false, openReader("./listInputWeird.txt"), b",,||,,"));

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
