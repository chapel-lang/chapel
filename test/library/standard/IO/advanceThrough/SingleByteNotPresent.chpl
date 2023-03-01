use IO;

testAdvancePast(openReader("singleByteInput.txt"), "-");
testAdvancePast(openReader("singleByteInput.txt"), b"-");

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
