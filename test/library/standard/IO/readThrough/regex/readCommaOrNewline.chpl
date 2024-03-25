use IO, List, Regex;

// string
const con = new regex("[,\\n]");
writeln(readIntoList(true, openReader("../listInput.txt", locking=false), con));
writeln(readIntoList(false, openReader("../listInput.txt", locking=false), con));

writeln(readIntoList(true, openReader("../listInputTrailing.txt", locking=false), con));
writeln(readIntoList(false, openReader("../listInputTrailing.txt", locking=false), con));

// bytes
const con_b = new regex(b"[,\\n]");
writeln(readIntoList(true, openReader("../listInput.txt", locking=false), con_b));
writeln(readIntoList(false, openReader("../listInput.txt", locking=false), con_b));

writeln(readIntoList(true, openReader("../listInputTrailing.txt", locking=false), con_b));
writeln(readIntoList(false, openReader("../listInputTrailing.txt", locking=false), con_b));

proc readIntoList(param reuseBuffer, r, separator: regex(?t)) where reuseBuffer == true {
  var s : t,
      l = new list(int);
  while r.readThrough(separator, s, stripSeparator=true) {
    l.pushBack(s:int);
  }
  return l;
}

proc readIntoList(param reuseBuffer, r, separator) where reuseBuffer == false {
  var l = new list(int);

  while true {
    try {
      const s = r.readThrough(separator, stripSeparator=true);
      l.pushBack(s:int);
    } catch {
      break;
    }
  }

  return l;
}
