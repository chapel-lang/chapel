use IO;

testAdvancePast(openReader("singleByteInput.txt"), ">");
testAdvancePast(openReader("singleByteInput.txt"), b">");

proc testAdvancePast(r, sep) {
  // advance past the delimiter
  r.advanceThrough(sep);

  // write the remainder of the file
  write(r.readAll());

  // try to advance from EOF (should throw 'EofError')
  try {
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }
}
