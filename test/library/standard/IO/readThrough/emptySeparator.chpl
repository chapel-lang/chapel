use IO;

proc testString() {
  const r = openReader("maxSizeInput.txt", locking=false);
  try {
      r.readThrough("");
  } catch e {
      writeln(e);
  }
  write(r.readAll());
}
testString();

proc testBytes() {
  const r = openReader("maxSizeInput.txt", locking=false);
  try {
      r.readThrough(b"");
  } catch e {
      writeln(e);
  }
  write(r.readAll());
}
testBytes();
