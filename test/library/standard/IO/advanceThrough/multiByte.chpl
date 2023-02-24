use IO;

testAdvancePast(openreader("multiByteInput.txt"), ">>>>abc>>>>");
testAdvancePast(openreader("multiByteInput.txt"), b">>>>abc>>>>");

proc testAdvancePast(r, sep) {
  // advance past the delimiter
  r.advanceThrough(sep);

  // write the remainder of the file
  write(r.readAll());

  // try to advance from EOF
  try {
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }
}
