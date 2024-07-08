use IO;

testAdvanceUpTo(openReader("singleByteInput.txt", locking=false), "-");
testAdvanceUpTo(openReader("singleByteInput.txt", locking=false), b"-");

proc testAdvanceUpTo(r, sep) {
  // try to read a separator that isn't in the channel (should throw 'UnexpectedEofError')
  try {
    r.advanceTo(sep);
  } catch e {
    writeln(e);
  }

  try {
    // try reading at EOF
    r.advanceTo(sep);
  } catch e {
    writeln(e);
  }
}
