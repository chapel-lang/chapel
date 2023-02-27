use IO;

testAdvanceUpTo(openreader("singleByteInput.txt"), "-");
testAdvanceUpTo(openreader("singleByteInput.txt"), b"-");

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
