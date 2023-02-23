use IO;

testAdvanceUpTo(openreader("multiByteInput.txt"), ">>>>abc>>>>");
testAdvanceUpTo(openreader("multiByteInput.txt"), b">>>>abc>>>>");

proc testAdvanceUpTo(r, sep) {
  // advance past the delimiter
  r.advanceUpTo(sep);

  // write the remainder of the file
  write(r.readAll());

  // try to advance from EOF
  try {
    r.advanceUpTo(sep);
  } catch e {
    writeln(e);
  }
}
