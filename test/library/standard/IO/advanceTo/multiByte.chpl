use IO;

testAdvanceUpTo(openReader("multiByteInput.txt", locking=false), ">>>>abc>>>>");
testAdvanceUpTo(openReader("multiByteInput.txt", locking=false), b">>>>abc>>>>");

proc testAdvanceUpTo(r, sep) {
  // advance past the delimiter
  r.advanceTo(sep);

  // write the remainder of the file
  write(r.readAll());

  // try to advance from EOF (should throw 'EofError')
  try {
    r.advanceTo(sep);
  } catch e {
    writeln(e);
  }
}
