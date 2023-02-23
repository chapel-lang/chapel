use IO;

testAdvancePast(openreader("singleByteInput.txt"), ">");
testAdvancePast(openreader("singleByteInput.txt"), b">");

proc testAdvancePast(r, sep) {
  // advance past the delimiter
  r.advancePast(sep);

  // write the remainder of the file
  write(r.readAll());

  // try to advance from EOF
  try {
    r.advancePast(sep);
  } catch e {
    writeln(e);
  }
}
