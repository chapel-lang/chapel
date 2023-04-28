use IO;

testAdvancePast(openReader("singleByteInput.txt"), "-");
testAdvancePast(openReader("singleByteInput.txt"), b"-");

proc testAdvancePast(r, sep) {
  // try to read a separator that's not in the channel (should throw 'UnexpectedEofError')
  try {
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }

  try {
    // try reading at EOF
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }
}
