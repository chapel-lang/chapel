use IO;

testAdvanceUpTo(openreader("multiByteInput.txt"), "<<<<123>>>>");
testAdvanceUpTo(openreader("multiByteInput.txt"), b"<<<<123>>>>");

proc testAdvanceUpTo(r, sep) {
  // try to read a delimiter not in the channel
  try {
    r.advanceUpTo(sep);
  } catch e {
    writeln(e);
  }

  // confirm pointer is at EOF
  write(r.readAll());
}
