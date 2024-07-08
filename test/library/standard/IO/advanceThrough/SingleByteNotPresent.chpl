use IO;

testAdvancePast(openReader("singleByteInput.txt", locking=false), "-");
testAdvancePast(openReader("singleByteInput.txt", locking=false), b"-");

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
