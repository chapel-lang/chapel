use IO;

testAdvancePast(openreader("multiByteInput.txt"), "<<<<123>>>>");
testAdvancePast(openreader("multiByteInput.txt"), b"<<<<123>>>>");

proc testAdvancePast(r, sep) {
  // try to read a delimiter not in the channel
  try {
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }

  // confirm pointer is at EOF
  write(r.readAll());
}
