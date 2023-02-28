use IO, List, Regex;

// string
const con = compile("[,\\n]");
writeln(readIntoList(true, openreader("../listInput.txt"), con));
writeln(readIntoList(false, openreader("../listInput.txt"), con));

writeln(readIntoList(true, openreader("../listInputTrailing.txt"), con));
writeln(readIntoList(false, openreader("../listInputTrailing.txt"), con));

// bytes
const con_b = compile(b"[,\\n]");
writeln(readIntoList(true, openreader("../listInput.txt"), con_b));
writeln(readIntoList(false, openreader("../listInput.txt"), con_b));

writeln(readIntoList(true, openreader("../listInputTrailing.txt"), con_b));
writeln(readIntoList(false, openreader("../listInputTrailing.txt"), con_b));

proc readIntoList(param reuseBuffer, r, separator: regex(?t)) where reuseBuffer == true {
  var s : t,
      l = new list(int);
  while r.readThrough(s, separator, stripSeparator=true) {
    l.append(s:int);
  }
  return l;
}

proc readIntoList(param reuseBuffer, r, separator) where reuseBuffer == false {
  var l = new list(int);

  while true {
    try {
      const s = r.readThrough(separator, stripSeparator=true);
      l.append(s:int);
    } catch {
      break;
    }
  }

  return l;
}
