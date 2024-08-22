use IO;

proc testString() {
  const r = openReader("pattern.txt", locking=false);
  try {
      r.readTo("");
  } catch e {
      writeln(e);
  }
  write(r.readAll());
}
testString();

proc testBytes() {
  const r = openReader("pattern.txt", locking=false);
  try {
      r.readTo(b"");
  } catch e {
      writeln(e);
  }
  write(r.readAll());
}
testBytes();
