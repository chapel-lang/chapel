use IO, List;

// string
writeln(readIntoList(true, openreader("./listInput.txt"), ","));
writeln(readIntoList(false, openreader("./listInput.txt"), ","));

writeln(readIntoList(true, openreader("./listInputTrailing.txt"), ","));
writeln(readIntoList(false, openreader("./listInputTrailing.txt"), ","));

writeln(readIntoList(true, openreader("./listInputWeird.txt"), ",,||,,"));
writeln(readIntoList(false, openreader("./listInputWeird.txt"), ",,||,,"));

// bytes
writeln(readIntoList(true, openreader("./listInput.txt"), b","));
writeln(readIntoList(false, openreader("./listInput.txt"), b","));

writeln(readIntoList(true, openreader("./listInputTrailing.txt"), b","));
writeln(readIntoList(false, openreader("./listInputTrailing.txt"), b","));

writeln(readIntoList(true, openreader("./listInputWeird.txt"), b",,||,,"));
writeln(readIntoList(false, openreader("./listInputWeird.txt"), b",,||,,"));

proc readIntoList(param reuseBuffer, r, separator: ?t) where reuseBuffer == true {
  var s : t,
      l = new list(int);
  while r.readThrough(s, separator, stripSeparator=true) {
    // take care of potential newlines (see '/regex/readCommaOrNewline.chpl')
    if !s.strip().isEmpty()
      then l.append(s:int);
  }
  return l;
}

proc readIntoList(param reuseBuffer, r, separator) where reuseBuffer == false {
  var l = new list(int);

  while true {
    try {
      // take care of potential newlines (see '/regex/readCommaOrNewline.chpl')
      const s = r.readThrough(separator, stripSeparator=true);
      if !s.strip().isEmpty() then l.append(s:int);
    } catch {
      break;
    }
  }

  return l;
}
