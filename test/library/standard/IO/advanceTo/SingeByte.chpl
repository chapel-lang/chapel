use IO;

testAdvanceUpTo(openreader("singleByteInput.txt"), ">");
testAdvanceUpTo(openreader("singleByteInput.txt"), b">");

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
