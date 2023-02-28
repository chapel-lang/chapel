use IO;

testAdvancePast(openreader("multiByteInput.txt"), "<<<<123>>>>");
testAdvancePast(openreader("multiByteInput.txt"), b"<<<<123>>>>");

proc testAdvancePast(r, sep) {
  // try to read a separator that's not in the channel (should throw EofError)
  try {
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }

  // confirm pointer is at EOF
  write(r.readAll());
}
