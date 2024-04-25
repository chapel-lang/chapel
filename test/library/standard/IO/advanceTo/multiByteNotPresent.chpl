use IO;

testAdvanceUpTo(openReader("multiByteInput.txt", locking=false), "<<<<123>>>>");
testAdvanceUpTo(openReader("multiByteInput.txt", locking=false), b"<<<<123>>>>");
testAdvanceUpTo(openReader("multiByteInput.txt", locking=false), "bbbb");
testAdvanceUpTo(openReader("multiByteInput.txt", locking=false), b"bbbb");

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
