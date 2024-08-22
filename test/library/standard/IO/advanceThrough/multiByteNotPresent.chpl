use IO;

testAdvancePast(openReader("multiByteInput.txt", locking=false), "<<<<123>>>>");
testAdvancePast(openReader("multiByteInput.txt", locking=false), b"<<<<123>>>>");
testAdvancePast(openReader("multiByteInput.txt", locking=false), "bbbb");
testAdvancePast(openReader("multiByteInput.txt", locking=false), b"bbbb");

proc testAdvancePast(r, sep) {
  // try to read a separator that's not in the channel (should throw 'UnexpectedEofError')
  try {
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }

  assert(r.atEOF());

  try {
    // try reading at EOF
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }
}
