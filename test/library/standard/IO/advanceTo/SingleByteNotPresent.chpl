use IO;

testAdvanceUpTo(openreader("singleByteInput.txt"), "-");
testAdvanceUpTo(openreader("singleByteInput.txt"), b"-");

proc testAdvanceUpTo(r, sep) {
  // try to read a separator that isn't in the channel (should throw 'EofError')
  try {
    r.advanceTo(sep);
  } catch e {
    writeln(e);
  }

  // confirm pointer is at EOF
  write(r.readAll());
}
