use IO;

testAdvancePast(openreader("singleByteInput.txt"), "-");
testAdvancePast(openreader("singleByteInput.txt"), b"-");

proc testAdvancePast(r, sep) {
  // try to read a delimiter not in the channel
  try {
    r.advancePast(sep);
  } catch e {
    writeln(e);
  }

  // confirm pointer is at EOF
  write(r.readAll());
}
